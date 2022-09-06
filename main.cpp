#include "clang/Tooling/Transformer/Transformer.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/RewriteRule.h"
#include "clang/Tooling/Transformer/Stencil.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Errc.h"
#include "llvm/Support/Error.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "version.h"

using namespace clang;
using namespace tooling;
using namespace ast_matchers;
using ::clang::transformer::cat;
using ::clang::transformer::changeTo;
using ::clang::transformer::makeRule;
using ::clang::transformer::member;
using ::clang::transformer::name;
using ::clang::transformer::insertBefore;
using ::clang::transformer::insertAfter;
using ::clang::transformer::statement;
using ::clang::transformer::statements;
using ::clang::transformer::node;

static llvm::cl::OptionCategory Category("clang-mute options");

bool applySourceChanges(const AtomicChanges& Changes) {
    std::set<std::string> Files;
    for (const auto &Change : Changes)
        Files.insert(Change.getFilePath());
    tooling::ApplyChangesSpec Spec;
    Spec.Cleanup = false;
    for (const auto &File : Files) {
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> BufferErr =
                llvm::MemoryBuffer::getFile(File);
        if (!BufferErr) {
            llvm::errs() << "error: failed to open " << File << " for rewriting\n";
            return true;
        }
        auto Result = tooling::applyAtomicChanges(File, (*BufferErr)->getBuffer(),
                                                  Changes, Spec);
        if (!Result) {
            llvm::errs() << toString(Result.takeError());
            return true;
        }
        std::error_code EC;
        llvm::raw_fd_ostream OS(File, EC, llvm::sys::fs::OF_TextWithCRLF);
        if (EC) {
            llvm::errs() << EC.message() << "\n";
            return true;
        }
        OS << *Result;
    }
    return false;
}

static void PrintVersion(raw_ostream &OS) {
  OS << "clang-mute " << CLANG_MUTE_VERSION << '\n';
}

int main(int argc, const char** argv) {
    llvm::cl::SetVersionPrinter(PrintVersion);
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, Category);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());

    int ErrorCount = 0;

    AtomicChanges Changes;

    auto Consumer = [&](Expected<MutableArrayRef<AtomicChange>> C) {
        if (C) {
            Changes.insert(Changes.end(), std::make_move_iterator(C->begin()),
                           std::make_move_iterator(C->end()));
        } else {
            llvm::errs() << "Error generating changes: "
                         << llvm::toString(C.takeError()) << "\n";
            ++ErrorCount;
        }
    };

    auto Rule = makeRule(functionDecl(hasBody(compoundStmt(hasDescendant(stmt()), isExpansionInMainFile()).bind("body"))),
                         flatten(insertBefore(statements("body"), cat("\n#if 0\n")),
                                 insertAfter(statements("body"), cat("\n#endif\n"))));

    Transformer Transformer(std::move(Rule), std::move(Consumer));

    MatchFinder Finder;
    Transformer.registerMatchers(&Finder);

    Tool.run(newFrontendActionFactory(&Finder).get());

    applySourceChanges(std::move(Changes));
}
