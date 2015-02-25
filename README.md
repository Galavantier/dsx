Dsx
===

Inspired by Facebooks' JSX and XHP projects,
 - JSX : http://facebook.github.io/react/docs/jsx-in-depth.html
 - XHP : https://www.facebook.com/notes/facebook-engineering/xhp-a-new-way-to-write-php/294003943919

The module takes XML/HTML strings and converts them into Drupal Render Arrays.
This lets Drupal modules define xml directly instead of using templates.
Allowing for much more modular designs, i.e.
 - Atomic design by Brad Frost (http://bradfrost.com/blog/post/atomic-web-design/),
 - or Google's card based design. (http://www.google.com/design/spec/components/cards.html)

DSX supports:
 - XML namespaces,
 - Interoperability with the full Drupal form API, including support for Drupal Form API elements
 - Is extensible via dynamic hooks for custom XML tags,
 - Or via the more traditional hook_element_info().

Requirements
============
 - Drupal 7.x
 - Does not support Drupal 8, but could probably be ported very easily.

Installation
==============
Install and Enable the module in Drupal.

There is no magic here; no hacking of Drupal core or custom PHP extensions. A fact I am quite proud of :-)

Usage
======
Any time you would return themed output,
use a Drupal Render Array,
or call `theme('template_name')`,
you can instead call `dsx_render(<DSX String>)`.

`dsx_render()` takes an xml string as input, and produces a Drupal Render Array as output.

*Note:* All text is sanitized and escaped using Drupal's built in sanitation functions before being added to the render array.

###PHP Heredoc Syntax
While `dsx_render()` can be used with normal PHP strings such as `dsx_render("<myElem>some tag</myElem>");`,
this can get very hard to manage with large blocks of xml, especially when string escaping is needed.

PHP has a lesser known feature called *Heredoc syntax* that makes dealing with large blocks of embedded text much more readable and convenient.

Heredoc strings are created by typing `<<<`, followed by an identifier, then a newline. The string itself follows, and then the same identifier again to close the quotation.

*Note:* For clarity, we will always use DSX as the Heredoc identifier in our code and examples.

Text inside a heredoc string *does not* need to be escaped, can contain newlines, and can be of arbitrary length.

Heredoc strings are treated like double-quoted strings, so you can use variables inside them using curly braces.

Here is an example:

    dsx_render(
    <<<DSX
    <!-- comment -->
    <test-elem class="test">
      <selfClosing prop="http://placekitten.com/g/200/300" />
      <div id="container">
        <h1 class="{$someVar}">Hello</h1>
      </div>
    </test-elem>
    DSX
    );


See the PHP docs for more details: http://php.net/manual/en/language.types.string.php#language.types.string.syntax.heredoc

###Arrays of XML
As a convenience, you can pass an array of DSX strings to `dsx_render()`.

Example:

    dsx_render(
      array(
        "<div>ElementA</div>",
        "<custom-elem class=\"awesome\">ElementB</custom-elem>",
        <<<DSX
        <wrapper>
          <h1>ElementC</h1>
        </wrapper>
        DSX
      )
    );

###Drupal Form API elements


Defining Custom Tags
====================

###Namespaces


###Dynamic hooks


###Traditional Drupal Method : hook_element_info()
