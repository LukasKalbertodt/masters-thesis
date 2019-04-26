# Customized Solarized Light Color Scheme

This is the theme I used in my LaTeX document (via minted). In order to make it usable with pygmentize, copy or link the `.py` file in the `styles/` folder, typically located here:

```
$HOME/.local/lib/python2.7/site-packages/pygments/styles
```

*Note*: `pygmentize -L` does not list the style afterwards, but it is still usable. (Try via `echo "fn main() {}" | pygmentize -l rust -O style=mysolarized`).
