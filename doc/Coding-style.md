# Edito coding style

## Code
- Maximum of 80 characters per line.
- Tabs are 4 characters long and indents are also 4 characters long. Tabs and indents are spaces, not \t.
- Types are read from right to left. For example : *char const \* const \** is a pointer to const pointer to const char.
- Always initialise variables.

## Comments
- Always document functions, files, struct, enum, ... with doxygen standard. The minimum is to use *@brief*, *@param*, *@return*. With functions, *@brief* starts with _"The <function_name>() function <verb>  ..."_.
- Only use /* */ comments (I'm not sure this is best practice. If someone with experience has arguments for moving to // comments, an issue can be opened on Github).

## Git
- Do not commit with whitespace errors. See *git diff --check* and https://github.com/dermesser/whitespace to fix whitespace errors.
- The commit message should start with a single line that’s no more than about 50 characters and that describes the set of modification concisely, followed by a blank line, followed by a more detailed explanation paragraph. Write your commit message in the imperative: "Fix bug" and not "Fixed bug" or "Fixes bug."
- Always rebase branches in local before pushing.
- Always merge remote branch. If a topic branches have diverged from main. Merge main into topic if necessary then merge topic into main.
