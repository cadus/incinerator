# State Machine

## Foo Bar

foobar

``` mermaid
graph LR
  A[Foo]
  B[Bar]
  C[Baz]
  D[Reset]
  A-->B
  B-->D
  B-->C
  A-- Enable -->C
  B-->A
  C-->D
  D-->C
  D-->B
  D-->A
```
