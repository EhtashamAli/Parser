/**
 * = Recursive descent parser =
 *
 * MIT Style License
 * By Dmitry Soshnikov <dmitry.soshnikov@gmail.com>
 *
 * In this short lecture we'll cover the basic (non-predictive, backtracking)
 * recursive descent parsing algorithm.
 *
 * Recursive descent is an LL parser: scan from left to right, doing
 * the left-most derivation.
 *
 * Top-down means it starts its analysis from the main start symbol, and goes
 * down to parsing the sub-parts of this start symbol. And the left-most
 * derivation means, that it tries to replace the left most non-terminal
 * in a production.
 *
 * Consider the grammar (the "->" symbol reads as "can be represented as"):
 *
 *   1. E -> T
 *   2. E -> T + E
 *   3. T -> a
 *   4. T -> a * T
 *   5. T -> (E)
 *
 * For brevity, usually the same non-terminal symbols are combined using the
 * alternative | symbol (reads as "or"), so we can represent the same grammar
 * shorter:
 *
 *   E -> T
 *      | T + E
 *
 *   T -> a
 *      | a * T
 *      | (E)
 *
 * An example source string which this language accepts is:
 *
 *   (a)
 *
 * And is derived as:
 *
 * E -> T -> (E) -> (T) -> (a)
 *
 * The recursive descent parsers are widely used on practice, since it's very
 * easy to implement them by hands, and the implementation just directly maps
 * the grammar to the code.
 *
 * The algorithm is simple:
 *
 *  (0. Start from the main top symbol of the grammar, E in our case)
 *
 *  1. For each non-terminal symbol in the grammar implement corresponding
 *     parsing function (so e.g. to parse E we'll have `function E() {}`)
 *
 *  2. If there are several alternative productions for a non-terminal,
 *     implement each sub-production as a sub-function, and try them in order
 *     If some of the sub-productions succeeds, the main non-terminal is
 *     considered succeeded as well. If a sub-production *does not* succeed,
 *     then *restore the cursor* (do backtracking) to the beginning of that
 *     sub-production, and try parsing next sub-production.
 *
 *     So for E we going to implement E1 for T, and E2 for T + E, and call
 *     E1 and E2 in order from main E:
 *
 *     E = E1 || E2;
 *
 *     E1 = T;
 *     E2 = T + E;
 *
 *  3. Implement function that checks for the presence of a needed token at
 *     the current cursor position in the source code. We'll call this function
 *     `term(...)`, and e.g. to check terminal "a" in the source stream at
 *     cursor position, we do term("a"). After the check (positive or negative)
 *     it also advances the cursor to the next token.
 *
 * Notice one important thing in the point (2): if during parsing of a
 * sub-production we realize that it won't succeed, we need to restore
 * (to backtrack) the cursor in order to try the next alternative. I.e. in this
 * version of recursive descent we don't do any prediction, but do full
 * backtracking in case of a fail. On practice a recursive descent usually
 * implements the predictive algorithm, which we'll cover in the next lectures.
 *
 * OK, enough theory, let's start to code! ;)
 */

/**
 * This tracks position of a current token.
 */
var cursor = 0;

/**
 * This is for restoring the cursor at backtracking. We save
 * the cursor every time before trying an alternative sub-production.
 */
var savedCursor = cursor;

// So according to point 0, we should start our parsing from the main top
// symbol, and then to go recursively down to sub-parts. In our case it's
// the symbol E, and as we see in the grammar it has two alternatives
// (T and T + E), which we encode as separate sub-functions E1 and E2.

/**
 * Main symbol E.
 *
 *   E -> T
 *      | T + E
 *
 * Succeeds if any first of its alternatives succeed. And as we mentioned,
 * we backtrack on each fail, in order to try the next alternative. For
 * that we have to save the cursor before trying the alternative.
 */
function E() {
  return (saveCursor(), E1()) || (backtrack(), saveCursor(), E2());
}

/**
 * E1 is the first alternative of E, and corresponds to the T.
 */
function E1() {
  return T();
}

/**
 * E2 is the second alternative of E, and corresponds to the T + E.
 *
 * Several interesting parts here:
 *
 * - E2 is going to succeed only if *all* of its sub-productions succeed
 *   (notice the usage of && operator for that).
 *
 * - We see this `term('+')` from the rule (3) of the parsing algorithm,
 *   that checks and expects the token "+" in the source stream, and
 *   fails if doesn't find this token there.
 *
 * - Here we first start to see the recursion for E(), hence the name
 *   of the parser: *recursive* descent.
 */
function E2() {
  return T() && term('+') && E();
}

// OK, we're done with E! Now let's encode the rules for T.

/**
 * Implements T non-terminal.
 *
 *   T -> a
 *      | a * T
 *      | (E)
 *
 * The same as E, it has alternatives: T1, T2, and T3.
 */
function T() {
  return (saveCursor(), T1()) ||
    (backtrack(), saveCursor(), T2()) ||
    (backtrack(), saveCursor(), T3());
}

/**
 * T1 alternative, T -> a
 */
function T1() {
  return term('a');
}

/**
 * T2 alternative, T -> a * T.
 *
 * We see two terminals go each after another, and again recursion for T.
 */
function T2() {
  return term('a') && term('*') && T();
}

/**
 * T3 alternative, T -> (E).
 *
 * Here we see a recursion even to the very first E symbol.
 */
function T3() {
  return term('(') && E() && term(')');
}

/**
 * Saves position of the current cursor for possible future backtracking.
 */
function saveCursor() {
  savedCursor = cursor;
}

/**
 * Restores cursor position to the saved one.
 */
function backtrack() {
  cursor = savedCursor;
}

/**
 * And here's our `term(...)` function, that checks for the
 * presence of a needed terminal in the stream, and advances
 * the cursor.
 */
function term(expected) {
  return getNextToken() === expected;
}

/**
 * Returns the next token in the stream, and advances the cursor.
 *
 * For simplicity we have only one-character tokens in this language
 * (the "a", "+", "(" and ")"), so we just return the next non-whitespace
 * symbol. And a real tokenizer returns tokens that may consist of several
 * characters, e.g. a token for keyword "int", or "while", etc.
 */
function getNextToken() {
  // Skip whitespace.
  while (source[cursor] === ' ') cursor++;
  var nextToken = source[cursor];
  cursor++;
  return nextToken;
}

/**
 * Stores the source.
 */
var source;

/**
 * Main parsing function, returns true if the string is accepted
 * by the grammar, or false otherwise.
 */
function parse(s) {
  source = s;
  cursor = 0;
  // We succeed if our main E symbol succeeds *and* we parse
  // all tokens (reached end of the source string).
  return E() && cursor == source.length;
}

// Test time!

console.log('(a)', parse('(a)')); // true
console.log('-a', parse('-a')); // false

// Seems legit so far. Now let's try parsing the:

// --- Simple backtracking limitation --

console.log('a * a', parse('a * a')); // false!

// Why is it false? It's clearly in the grammar of our language.
// Is it a bug in the parser? Not really.

// Here we encounter the first limitation of the recursive descent parsing.
// In fact, the backtracking algorithm we implemented here is kind of "naive":
// the parser succeeded when it found first "a" token (since it corresponds to
// the T1 alternative of T), and then it stopped parsing. However, we haven't
// yet reached the end of the string. Had we tried the T2 production first
// (a * T), the string would be parsed. So the conclusion which we can make
// here is:
//
//   - This implementation of the recursive descent is capable to parse
//     only productions with one alternative that starts with the same token
//     (it cannot parse "a" and "a * T" if tries just "a" first). So the
//     backtracking algorithm we have here is *not generic*.
//
//  - However, it's possible to implement a generic backtracking that would
//    consider as well the fact, that the cursor hasn't reached yet the
//    end of the source string, and that we should probably try another
//    alternative (even if the previous alternative succeeded!), T2 in our
//    case, that would parse the "a * a".
//
//  - Another approach to fix it, is to restructure our grammar. This
//    technique is called *left factoring*, which we wil cover in later
//    lectures.

// Additional info can be found in the "Dragon book", and in other lectures
// e.g from prof. Alex Aiken.

// Exercise: Implement generic backtracking that would parse "a + a".