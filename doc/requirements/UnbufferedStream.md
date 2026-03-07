# BufferedStream

## Abstract:

One type that has to satisfy an unbuffered Stream operator and type.

## Rationale:

- Fast and direct-flush operations.
- High-Throughput systems.

## Example:

```cpp
UBStream<UBOperator, UBType>;
```

UBOperator must flush at all times!

