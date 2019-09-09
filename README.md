## WordCompletion

The `wordCompletion` class stores a dictionary `D` of word-frequency pairs. Each word in `D` has a unique *identifier* (a non-negative integer) that corresponds to the insertion order of words. The dictionary supports the following operations:

1. `int access(string w)`: If `w` is not in `D`, insert `w` with frequency `1` into `D`, and assign the smallest unused identifier to `w`; otherwise, increase its frequency by `1` and the identifier is unchanged. Either way, return the identifier of `w`.
2. `std::vector<std::vector<int>> getCompletions(string w, int k)`: This finds, for all `i = 0, ..., |w|`, the `k` most frequent word completions for `w[0, ..., i-1]`.

The implementation uses a variation of range trees consisting of trie and array-based heaps. 