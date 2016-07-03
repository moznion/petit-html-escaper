petit-html-escaper
==

[![Build Status](https://travis-ci.org/moznion/petit-html-escaper.svg)](https://travis-ci.org/moznion/petit-html-escaper)

A simple and small escaper for HTML with SSE4.2 function.

AIMs
--

- Make HTML escaping faster!
- Support escaping special characters which are used by JavaScript frameworks (e.g. AngularJs and etc)

Benchmark
--

Result:

```
petit-html-escaper: 3.935205 [sec]
simple-impl: 5.634651 [sec]

petit-html-escaper is faster 143.185715% than simple implementation
```

If you try to execute benchmarking on your environment, you can run the benchmarking by the following command:

```
$ cmake . && make bench
```

For Developers
--

### Test

```sh
cmake . && make test
```

### Test without SSE

```sh
DISABLE_SSE=1 cmake . && make test
```

Author
--

moznion (<moznion@gmail.com>)

mattn

License
--

```
The MIT License (MIT)
Copyright © 2015 moznion, http://moznion.net/ <moznion@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```
