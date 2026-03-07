# BufferedStream

## Abstract:

One type that has to satisfy a BufferedStream operator and type.

## Rationale:

- HPC systems.
- I/O systems.
- HFT systems.

## Example:

```cpp
BStream<BOperator, BType>;
```

BOperator must not flush at all times, but have the option to.
