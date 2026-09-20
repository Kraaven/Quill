---
slug: quill-validation-fix
status: drafting
intent: clear
review_required: false
pending-action: write .omo/plans/quill-validation-fix.md
approach: Fix the fstat FILE*-as-fd crash in jsonFileValidation.h, repair the extension check, give main a working success path (log + exit codes + wired parser stub), and split the header-defined function into .c/.h to remove the latent linker bomb.
---

# Draft: quill-validation-fix

## Components (topology ledger)

| id | outcome | status | evidence |
|----|---------|--------|----------|
| cli-args | main.c arg count check works (no args / too many) | active | run: "No Json File Provided" ok |
| file-validation | ValidateJsonFile crashes on VALID files, misclassifies extensions | active | run: exit 0x80000003 on hello.json; README.md/misclass paths behave |
| parser | ParseJsonFile stub exists but is unreachable: no prototype (parser.h empty), never called, leaks FILE* | active | Src/parser.c:5-9, Src/parser.h (empty) |
| build-system | CMake/VS build works, /W4 warning about fstat call was emitted and ignored | active | Build/quill.exe newer than all sources; CMakeLists.txt:34-38 |

## Open assumptions (announced defaults)

| assumption | adopted default | rationale | reversible? |
|------------|-----------------|-----------|-------------|
| extension match semantics | ".json" must be at END of name, case-insensitive | Windows extensions are case-insensitive; strstr anywhere matches "foo.json.bak" | yes |
| exit codes | 0 on success, non-zero (1) on validation failure | success currently exits 0 with zero output; failures also exit 0 | yes |
| validation code placement | move ValidateJsonFile body to Src/jsonFileValidation.c, keep prototype in .h | header-defined function = multiple-definition linker error once a second .c includes it | yes |
| success path | main calls ParseJsonFile(argv[1]) and logs result; stub still returns true but closes the file | the program currently does NOTHING on success — part of "not working" | yes |
| no new test framework | QA = run binary over the 3 real cases + exit-code checks via cmake target | CTest scaffolding exists but adding a framework is unrequested | yes |

## Findings (cited - path:lines)

1. **ROOT CAUSE — crash on valid input.** `Src/jsonFileValidation.h:26` calls `fstat(file, &sb)` with a `FILE*` where `fstat` requires an `int` file descriptor. A `FILE*` is not an fd; the CRT receives a garbage descriptor and the process dies with an exception (exit code 0x80000003 STATUS_BREAKPOINT) before returning — so no log prints and the user sees silent failure. Correct call: `fstat(fileno(file), &sb)` (MSVC: `_fileno`). Evidence: reproduced 3x via `Build\quill.exe Build\hello.json` → exit -2147483645, zero output after "Executing Quill".
2. **Failure asymmetry proves the branch.** README.md and doesnotexist.json return BEFORE the fstat line and log correctly; only the path that reaches fstat (valid, existing .json) crashes. Run log: exit codes 0 / 0 / 0x80000003 respectively.
3. **fstat error message is wrong.** `Src/jsonFileValidation.h:27` logs "File %s is empty" on fstat failure — fstat failing means syscall failure, not emptiness; and `sb.st_size` is never checked even on success, so an actual empty-file requirement is not enforced.
4. **Extension check matches anywhere.** `Src/jsonFileValidation.h:11` uses `strstr(fileName, ".json")` — "notes.json.bak", "foo.jsonx" would pass. Needs end-of-string (extension) check.
5. **Success path is silent and dead.** `Src/main.c:22-26`: after validation succeeds, main falls off the end — no final `return`, no success log, no parse call. Combined with finding 1, "not working" = valid files crash; if that were fixed the program would still do nothing.
6. **Parser is disconnected.** `Src/parser.h` is empty (no prototype for `ParseJsonFile`); `Src/parser.c:6` opens the file, never reads or closes it, returns true. Nothing in main.c calls it. Modern compilers reject undeclared function calls, so wiring it requires the prototype.
7. **Latent linker bomb.** `ValidateJsonFile` is fully defined in a header (`Src/jsonFileValidation.h:9-33`). Adding a second .c that includes it → multiple definition errors. Currently only main.c includes it.
8. **Build is fresh and current.** Build/quill.exe (22:34:34) is newer than jsonFileValidation.h (22:34:27) and main.c (18:59:10); binary prints the new LOG strings, so diagnosis is against current code, not a stale build.
9. **Compiler already flagged it.** CMake uses /W4 (`CMakeLists.txt:35`); the FILE*→int conversion emits a warning the build ignored. Not a config bug — do not propose /WX.

## Decisions (with rationale)

- Diagnose-by-reproduction: ran the binary with 3 input classes; branch map is conclusive; no user interview needed (intent: CLEAR — "why is it not working" has a deterministic, reproduced answer).
- Fix fstat via fileno (not fseek/ftell): smallest change preserving the stat-based design; portable across MSVC/GCC (`fileno` on POSIX, `_fileno` on MSVC with fallback define).
- Keep ValidateJsonFile's interface unchanged; move implementation to .c file.
- Wire ParseJsonFile minimally (prototype + call + log + fclose). Do NOT implement the real parser — that is the user's next feature, not a bug fix.

## Scope IN

- Fix `fstat` misuse → `fstat(fileno(file), &sb)` with MSVC/POSIX portability shim.
- Fix extension check: extension at end-of-string, case-insensitive on Windows.
- Fix misleading empty-file log; check `sb.st_size == 0` if a real empty-file rejection is wanted (adopt: yes — the message already claims that behavior).
- Add final `return 0;` in main + non-zero exit code on validation failure; log a success line.
- Create `Src/parser.h` prototype; call `ParseJsonFile(argv[1])` from main after validation; fclose in parser stub.
- Move `ValidateJsonFile` definition into new `Src/jsonFileValidation.c`; header keeps only prototype + includes.
- Rebuild and verify all 3 cases + a real empty .json case; log evidence.

## Scope OUT (Must NOT have)

- No JSON grammar/tokenizer implementation (parser stays a stub; unrequested).
- No new dependencies, test frameworks, or CTest wiring beyond what exists.
- No /WX or warning-policy changes to CMakeLists.txt.
- No refactor of logging.h beyond leaving behavior identical.

## Open questions

None — failure fully reproduced and root-caused; remaining choices are announced defaults above.

## Approval gate

status: awaiting-approval
next-workflow-action: on user okay, rerun scaffold without --draft-only to create .omo/plans/quill-validation-fix.md and append task batches