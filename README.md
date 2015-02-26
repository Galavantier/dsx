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

 - Standard html tags become render arrays of type `html_tag`:

 `<h1 id="header">test</h1>` becomes `array('#type' => 'html_tag', '#tag' => 'h1', '#attributes' => array('id' => 'header'), '#value' => 'test')`

 - Non standard tags are converted to render arrays using their appropriate render function.
 - DSX custom tag Render functions should always return a render array, either by using `render_dsx()` themselves, or by generating a render array directly. See the custom tags section below for a more detailed explanation.

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

```PHP
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
```

See the PHP docs for more details:
http://php.net/manual/en/language.types.string.php#language.types.string.syntax.heredoc

###Arrays of XML
As a convenience, you can pass an array of DSX strings to `dsx_render()`.

Example:
```PHP
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
```
###Drupal Form API elements
DSX makes the entire Drupal form API available under the `drupal` XML namespace. (See below for more details about XML namespaces)

DSX translates the tags as follows:
 - The tag name becomes the #type in the render away. 'drupal:password' becomes array('#type' => 'password')

See the Drupal Form API for a list of all elements and attributes available:
https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7

Defining Custom Tags
====================

###Namespaces


###Dynamic hooks


###Traditional Drupal Method : hook_element_info()

Tips, Tricks, and Caveats
=========================
###Remember: DSX is just syntactic sugar on top of render arrays.

 - You can mix and match DSX and render arrays. This can be very useful for integration with other Drupal code,
such as hook_form definitions.

Example (This is a contrived example; you would normally just define the whole form in DSX, but it illustrates the point):
```PHP
function my_module_example_form($form, &$form_state) {
  $form['some-textarea'] = dsx_render(
<<<DSX
<drupal:textarea title="a big text area" description="asdfasdf" id="my-longtext">default text</drupal:textarea>
DSX
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}
```
