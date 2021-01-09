# Python Bindings

## Launching Program w/ Python Library

- Make sure `libarpirobot-core.so` is in the same directory as `main.py`
- Make sure `arpirobot` folder (python bindings) is in same directory as `main.py`

```
sudo LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH PYTHONPATH=. python3 main.py
```