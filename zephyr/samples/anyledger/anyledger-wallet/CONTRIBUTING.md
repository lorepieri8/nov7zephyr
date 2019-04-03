<meta charset="utf-8"> 

# Contributing to AnyLedger wallet
We love your input! We want to make contributing to this project as easy and transparent as possible, whether it's:

- Reporting a bug
- Discussing the current state of the code
- Submitting a fix
- Proposing new features

## We Develop with Github
We use github to host code, to track issues and feature requests, as well as accept pull requests. Please report bugs using Github's issues.

## We Use [Github Flow](https://guides.github.com/introduction/flow/index.html), So All Code Changes Happen Through Pull Requests
Pull requests are the best way to propose changes to the codebase (we use [Github Flow](https://guides.github.com/introduction/flow/index.html)). We actively welcome your pull requests.
Before you start writing the code, please make yourself familiar with [JPL Coding standard][1] and [The power of ten][2]. Even though not all rules from the standard will be enforced, it will give you a good idea of what is expected from the code before it can be merged.

1. Fork the repo and create your branch from `master`.
2. If you've added code that should be tested, add tests.
3. If you've changed APIs, update the documentation.
4. Ensure the test suite passes.
5. Make sure your code lints.
6. Issue that pull request!

All pull requests require the code to be reviewed and approved by another person in the team.


## Write bug reports with detail, background, and sample code
[This is an example](http://stackoverflow.com/q/12488905/180626). Here's [another example](http://www.openradar.me/11905408).

**Great Bug Reports** tend to have:

- A quick summary and/or background
- Steps to reproduce
  - Be specific!
  - Give sample code if you can.
- What you expected would happen
- What actually happens
- Notes (possibly including why you think this might be happening, or stuff you tried that didn't work)

##  Share Early, Share Often

We firmly believe in the share early, share often approach.  The basic premise
of the approach is to announce your plans **before** you start work, and once
you have started working, craft your changes into a stream of small and easily
reviewable commits.

This approach has several benefits:

- Announcing your plans to work on a feature **before** you begin work avoids
  duplicate work.
- It permits discussions which can help you achieve your goals in a way that is
  consistent with the existing architecture.
- It minimizes the chances of you spending time and energy on a change that
  might not fit with the consensus of the community or existing architecture and
  potentially be rejected as a result.
- Incremental development helps ensure you are on the right track with regards
  to the rest of the community.
- The quicker your changes are merged to master, the less time you will need to
  spend rebasing and otherwise trying to keep up with the main code base.


##  Code Documentation and Commenting

- At a minimum every function must be commented with its intended purpose and
  any assumptions that it makes
  - The general rule of thumb is to look at it as if you were completely
    unfamiliar with the code and ask yourself, would this give me enough
    information to understand what this function does and how I'd probably want
    to use it?
- Exported functions should also include detailed information the caller of the
  function will likely need to know and/or understand:
  
 **WRONG**
```Go
// convert a compact uint32 to big.Int
func CompactToBig(compact uint32) *big.Int {
```
**RIGHT**
```Go
// CompactToBig converts a compact representation of a whole number N to a
// big integer.  The representation is similar to IEEE754 floating point
// numbers.
//
// Like IEEE754 floating point, there are three basic components: the sign,
// the exponent, and the mantissa. They are broken out as follows:
//
//        * the most significant 8 bits represent the unsigned base 256 exponent
//        * bit 23 (the 24th bit) represents the sign bit
//        * the least significant 23 bits represent the mantissa
//
//        -------------------------------------------------
//        |   Exponent     |    Sign    |    Mantissa     |
//        -------------------------------------------------
//        | 8 bits [31-24] | 1 bit [23] | 23 bits [22-00] |
//        -------------------------------------------------
//
// The formula to calculate N is:
//         N = (-1^sign) * mantissa * 256^(exponent-3)
//
// This compact form is only used in bitcoin to encode unsigned 256-bit numbers
// which represent difficulty targets, thus there really is not a need for a
// sign bit, but it is implemented here to stay consistent with bitcoind.
func CompactToBig(compact uint32) *big.Int {
```
- Comments in the body of the code are highly encouraged, but they should
  explain the intention of the code as opposed to just calling out the
  obvious
  
**WRONG**
```Go
// return err if amt is less than 5460
if amt < 5460 {
	return err
}
```
**RIGHT**
```Go
// Treat transactions with amounts less than the amount which is considered dust
// as non-standard.
if amt < 5460 {
	return err
}
```
**NOTE:** The above should really use a constant as opposed to a magic number,
but it was left as a magic number to show how much of a difference a good
comment can make.


## Model Git Commit Messages

This project prefers to keep a clean commit history with well-formed commit
messages.  This section illustrates a model commit message and provides a bit
of background for it.  This content was originally created by Tim Pope and made
available on his website, however that website is no longer active, so it is
being provided here.

Here?s a model Git commit message:

```
prefix: Short summary of changes (max 50 chars)

More detailed explanatory text, if necessary.  Wrap it to about 72
characters or so.  In some contexts, the first line is treated as the
subject of an email and the rest of the text as the body.  The blank
line separating the summary from the body is critical (unless you omit
the body entirely); tools like rebase can get confused if you run the
two together.

Write your commit message in the present tense: "Fix bug" and not "Fixed
bug."  This convention matches up with commit messages generated by
commands like git merge and git revert.

Further paragraphs come after blank lines.

- Bullet points are okay, too
- Typically a hyphen or asterisk is used for the bullet, preceded by a
  single space, with blank lines in between, but conventions vary here
- Use a hanging indent
```

The commit prefix is always of the form `prefix: `. It is for the sole
purpose of indicating which package or component was touched in a
commit.

Here is how the right prefix for a commit is chosen.
- If a commit modifies a component in the main package
  (eg. `blocklogger`) use the component name as the commit prefix.
- If a commit modifies a component in any of the packages besides the main
  package use the package name as the commit prefix (eg. `dcrjson`).
- If a commit modifies components in multiple packages use the word `multi`
  as the commit prefix.

## Red flags

### Compiler warnings
The CI compiler has all warnings enabled and will fail if there is one. Do not silence the warnings with `#pragma` or similar. If you feel there's a reason to supress the warning, please discuss the issue with project maintainer. (Also see: JPL-LOC1.2)

### Dynamic memory allocation
As we are targeting embedded platforms, there shall be no dynamic memory allocation after the task is initialized. (Also see: JPL-LOC2.5)

### No asserts in your code
Use static and dynamic asserts as a sanity checks. It's always better to fail early. (Also see: JPL-LOC3.16)

### Changes to the build subsystem
Please do not change the build/CI configuration without discussing the issue with the team. This also applies to any requirements or additional libraries.


## Any contributions you make will be under the Apache-2.0 Software License
In short, when you submit code changes, your submissions are understood to be under the [Apache-2.0 License](https://choosealicense.com/licenses/apache-2.0/).

## References
This document was adapted from the open-source contribution guidelines for [Facebook's Draft](https://github.com/facebook/draft-js/blob/a9316a723f9e918afde44dea68b5f9f39b7d9b00/CONTRIBUTING.md)
[JPL Coding standard](https://lars-lab.jpl.nasa.gov/JPL_Coding_Standard_C.pdf) and from [Decred](https://github.com/decred/dcrd/blob/master/docs/code_contribution_guidelines.md).

[1]: https://lars-lab.jpl.nasa.gov/JPL_Coding_Standard_C.pdf
[2]: https://web.cecs.pdx.edu/~kimchris/cs201/handouts/The%20Power%20of%2010%20-%20Rules%20for%20Developing%20Safety%20Critical%20Code.pdf
[3]: https://github.com/decred/dcrd/blob/master/docs/code_contribution_guidelines.md