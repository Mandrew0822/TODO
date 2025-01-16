# TODO
my personal, and very basic, todo program.

its not meant to be used by others, but feel free to if you like :)

## Features

- Easy to add and remove items
- Optional terminal startup integration
- Supports both bash and zsh shells

## Installation

```bash
git clone https://github.com/Mandrew0822/TODO.git
cd TODO
chmod +x build
./build
```

## Usage

### Add a todo item
```bash
todo buy groceries
```

### View your todo list
```bash
todo
```
Output example:
```
TODO LIST:
----------
1. buy groceries
2. call mom
3. finish project
----------
```

### Remove a todo item
```bash
todo 2    # Removes the second item from the list
```
