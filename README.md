## My Custom Maximum Manhattan Distance

```
git clone https://github.com/schneems/max_manhattan_distance
```

Then in vagrant run:

```
make clean ; make && ./manhattan
```

Keep pressing enter to generate maximum manhattan distance. To exit hit `CTRL+C`.

## Args

 Two arguments are provide `-p` for probability, choose a value between 1 and 99 for the likelyhood any cell will contain a 1.

 The next argument `-w` sets the width of the matrix.

```
$ make clean ; make && ./manhattan -w 10 -p 2
closest points:
  rank: 2 (1, 1)
  rank: 10 (1, 1)
  rank: 7 (4, 1)
  rank: 2 (1, 1)
[
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
  [ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 ]
]
Maximum manhattan distance: 14
```

If no points are provided the output is not specified