# Unbracify
Do you love python's whitespace block formatting?
Do you wish every other language also used it?
No? I made it anyway.

Unbracify is a source-to-source compiler, that allows you to bring python style blocks anywhere.

An yes, unbracify is implemented in itself!

## Examples

### C
```c
#include <stdio.h>

int main():
    printf("Look ma! No braces!!\n");
    return 1;
```

### Rust
```rust
use std::env;

fn main():
    let mut args = env::args();
    args.next().unwrap();
    if args.next().unwrap_or_default() == "I love braces":
        println!("Boo!");
    else:
        println!("Yay!");
```

### Bython
```python
def main():
    print("Peak technology")

if __name__ == '__main__':
    main()
```
