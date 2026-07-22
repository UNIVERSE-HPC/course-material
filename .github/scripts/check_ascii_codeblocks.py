#!/usr/bin/env python3
"""Fail if any fenced code block in the repo's Markdown files contains
non-ASCII characters, unless the block opts out with an `allow-non-ascii`
flag in its opening fence info string, e.g.:

    ```python allow-non-ascii
    name = "café"
    ```

`text` and `output` blocks are exempt, since they typically hold captured
terminal output or illustrative pseudocode rather than real code.
"""

import re
import subprocess
import sys

FENCE_RE = re.compile(r"^( {0,3})(`{3,}|~{3,})(.*)$")
OPT_OUT_FLAG = "allow-non-ascii"
EXEMPT_LANGUAGES = {"text", "output"}


def list_markdown_files() -> list[str]:
    result = subprocess.run(
        ["git", "ls-files", "*.md"],
        capture_output=True,
        text=True,
        check=True,
    )
    return [line for line in result.stdout.splitlines() if line]


def find_violations(path: str) -> list[str]:
    violations = []

    with open(path, encoding="utf-8") as f:
        lines = f.readlines()

    in_block = False
    fence_char = ""
    fence_len = 0
    fence_lineno = 0
    skip_block = False
    lang = ""

    for lineno, raw_line in enumerate(lines, start=1):
        line = raw_line.rstrip("\n")
        match = FENCE_RE.match(line)

        if not in_block:
            if match:
                fence_char = match.group(2)[0]
                fence_len = len(match.group(2))
                fence_lineno = lineno
                tokens = match.group(3).strip().split()
                lang = tokens[0].lower() if tokens else "(none)"
                skip_block = lang in EXEMPT_LANGUAGES or OPT_OUT_FLAG in tokens[1:]
                in_block = True
            continue

        # Inside a block: check for a closing fence first.
        if match and match.group(2)[0] == fence_char and len(match.group(2)) >= fence_len and not match.group(3).strip():
            in_block = False
            continue

        if skip_block:
            continue

        for col, char in enumerate(line, start=1):
            if ord(char) > 127:
                violations.append(
                    f"{path}:{lineno}:{col}: non-ASCII character {char!r} "
                    f"(U+{ord(char):04X}) in fenced code block opened at "
                    f"{path}:{fence_lineno} (lang={lang}). "
                    f"If deliberate, add `{OPT_OUT_FLAG}` to that fence, "
                    f"e.g. ```{lang} {OPT_OUT_FLAG}"
                )

    return violations


def main() -> int:
    all_violations = []
    for path in list_markdown_files():
        all_violations.extend(find_violations(path))

    if all_violations:
        print("Non-ASCII characters found in fenced code blocks:\n")
        for violation in all_violations:
            print(violation)
        print(f"\n{len(all_violations)} violation(s) found.")
        return 1

    print("No non-ASCII characters found in fenced code blocks.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
