## randwords

---

### Description

Command-line tool that generates a series of random words for Morse code practice, and prints to standard output.

Words are chosen from a selected subset of 5000 common English words.

Runs on Mac or Linux.

### Usage

Use **-c** option to choose number of words per row.

Use **-n** option to choose total number of words.

Use **-f** option to choose starting point in list of words, e.g., **-f 200** means start using words from the 200th most common word.

Use **-t** option to choose ending point in list of words, e.g., **-t 500** means use words up to the 500th most common word.

Combine with **mbeep** to play Morse code. See example below.

For more information, see man page.

### Build and install

* macOS

```
cd path_to_directory
make
make install
```

* Linux

```
cd path_to_directory
make
sudo make install
```

### Examples

Print 50 words selected from the most common 100 English words, with 10 words per row:

```
randwords -t 100 -n 50 -c 10
```

Play Morse code for 80 words selected from the 100th to 200th most common English words:

```
randwords -f 100 -t 200 -n 80 | mbeep -i /dev/stdin -c
```

### License

Mostly BSD

Portions CC-BY, CC0
