# Test content exp6

```sh
cat - # pipe stdin to stdout
```

## Hotkeys

- F12: toggle input mask
- F11: toggle output mask

## Input

- `content` can be any string.
- `<F12>`, `<F11>`: Press key here.

```txt
Normal: content
F12: <F12>content
<F12>F11: <F11>content
F12+F11: <F12>content
```

## Console Display

```txt
Normal: content
Normal: content
F12: *******
F12: content
F11: content
***: *******
F12+F11: *******
***+***: *******
```
