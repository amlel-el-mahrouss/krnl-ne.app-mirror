-----------------------------

# 0: Basic DSL Rules.

-----------------------------

## Comments and Pragmas

- **Comments**: Lines starting with `#` followed by text are treated as comments or pragmas
  ```lcp
  # This is a comment
  ```

- **Pragma directives**: Lines starting with `# --` are pragma directives
  ```lcp
  # --pragma-test          # Marks file as test
  # --test-list-apps       # Defines a test case
  # --test-start           # Defines a start test case
  ```

## Iteration

- **For loops**: Iterate over collections using begin/end iterator pairs
  ```lcp
  for <iterator>: (<begin>, <end>) {
      <body>
  }
  ```
  Example:
  ```lcp
  for it: (apps.begin, apps.end) {
      it.print();
  }
  ```

- **Iterator methods**: Iterators support method calls using dot notation
  ```lcp
  it.print();    # Call print method on iterator
  ```

## Event Handlers

- **When statements**: Define event handlers triggered by specific events
  ```lcp
  when <event>: (<arguments>) {
      <body>
  }
  ```
  Example:
  ```lcp
  when start: (apps) {
      it (apps);
  }
  ```

## Collections

- **Collection access**: Collections provide `.begin` and `.end` properties for iteration
  ```lcp
  apps.begin     # Start iterator
  apps.end       # End iterator
  ```

## Syntax Rules

- **Whitespace**: Indentation is used for readability but not syntactically significant
- **Blocks**: Code blocks are delimited by curly braces `{ }`
- **Statements**: End with semicolon `;` or newline
- **Parentheses**: Used for grouping arguments and parameters
- **Colon**: Separates declaration from parameters in control structures

## Identifiers

- **Naming**: Identifiers can contain letters, numbers, and underscores
- **Case sensitivity**: Identifiers are case-sensitive
- **Reserved keywords**: `for`, `when`, `it`

## Types

- **Collections**: `apps` (application collection)
- **Iterators**: Created via `for` loops or explicit `it()` constructor
- **Events**: `start` (startup event)

