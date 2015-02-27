DSX
===

Inspired by Facebooks' JSX and XHP projects,
 - JSX : http://facebook.github.io/react/docs/jsx-in-depth.html
 - XHP : https://www.facebook.com/notes/facebook-engineering/xhp-a-new-way-to-write-php/294003943919

The module takes XML/HTML strings and converts them into Drupal Render Arrays.
This lets Drupal modules define xml as output instead of using templates or theming functions.

The goal is to create a fine grained modular workflow in the Drupal environment;
emphasizing component based design over strict MVC pattern, where logic and display are grouped
together in small concrete units that can be easily composed together.

Allowing for much more modular designs, i.e.
 - Atomic design by Brad Frost (http://bradfrost.com/blog/post/atomic-web-design/)
 - or Google's card based design. (http://www.google.com/design/spec/components/cards.html)

DSX supports:
 - XML namespaces
 - Full Interoperability with Drupal render arrays, including the full Drupal form API and custom elements defined by `hook_element_info()`
 - Is extensible via dynamic hooks for custom XML tags

Requirements
============
 - PHP >= 5.3.6
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
 - DSX custom render functions should always return a render array, either by using `render_dsx()` themselves, or by generating a render array directly.
 See the custom tags section below for more detail.

 - By default the render arrays are numerically indexed. If you need named keys, such as for use with Drupal forms, all tags optionally support the `name` attribute:

  `<h1 id="header" name="myHeader">test</h1>` becomes `array( 'myHeader' => array('#type' => 'html_tag', '#tag' => 'h1', '#attributes' => array('id' => 'header'), '#value' => 'test') )`

*Notes:*
 - All text is sanitized and escaped using Drupal's built in sanitation functions before being added to the render array.
 - Unlike proper XML, you do not have to have a *root* or parent tag to contain the xml elements. You can just give DSX an arbitrary group of tags. This is useful for custom components.

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

DSX translates namespaced tags as follows:
 - The tag name becomes the #type in the render away.

  Example: `<drupal:password />` becomes `array('#type' => 'password')`

 - All tag attributes become `#` prefixed keys in the render array,
 which correspond to form api attributes

  Example: `<drupal:password size="15" />` becomes `array('#type' => 'password', '#size' => 15)`

 - For convenience, the following attributes are exceptions that are parsed differently:
    - `id`, `enctype`, and `lang` are added as subkeys to the `#attributes` key in the render array.

    - `class` is added to the `#attributes` key after it is parsed into an array of class names.

        Example:
        `<drupal:password size="15" id="myPass" class="my-pass pass" />` becomes:

        ```PHP
        array(
          '#type' => 'password',
          '#size' => 15,
          '#attributes' => array(
            'id' => 'myPass',
            'class' => array('my-pass', 'pass')
          )
        )
        ```

    - The `#attribute` key can be set directly by defining the `attribute` property on the tag with a JSON formatted string representing the the attributes configuration.

        Example:
        `<drupal:password size="15" attributes="{'id':'myPass', class:['my-pass', 'pass']}" />` becomes:

        ```PHP
        array(
          '#type' => 'password',
          '#size' => 15,
          '#attributes' => array(
            'id' => 'myPass',
            'class' => array('my-pass', 'pass')
          )
        )
        ```
See the Drupal Form API for a list of all elements and attributes available:
https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7

###Non-Strict Mode
By Default, DSX will error if it is unable to find a render function or element definition for a custom tag.
Normally this is a good thing, but when developing large projects with possibly unfinished components, it can be annoying.

Running DSX in Non-Strict mode will cause DSX to fail gracefully for undefined custom components.
DSX will simply treat the custom component as a standard HTML tag, and output the tag as it is written.

Non-Strict mode is enabled by passing FALSE as the second parameter to `dsx_render()`

Example:
```PHP
dsx_render(
<<<DSX
<undefined-elem>
    <h1>Hello</h1>
</undefined-elem>
DSX
, FALSE); // Turn Strict OFF
```

will produce a render array that looks like this:
```PHP
array(
  '#type' => 'html_tag',
  '#tag' => 'undefined-elem'
  0 => array(
        '#type' => 'html_tag',
        '#tag' => 'h1',
        '#value' => 'Hello'
      )
);
```

Defining Custom Tags
====================
One of the most important features of DSX is that it allows for the quick creation of custom
components that can then be composed or nested together to create bigger components.

This allows for a workflow that closely mimics React.js and atomic design
principles inside a Drupal environment.

Traditional template files end up living very far away from the context in which they
are used. This tends to lead to either templates that are very big and inflexible,
or templates that are so small that is becomes hard to determine their context.

With DSX you define semantically meaningful components dynamically as you
need them in the context where they are needed; no need to register themes with a massive index
function, or manage many template files that don't compose well or describe their context.


###Namespaces
DSX supports the XML namespace syntax: `<namespace:tagname />`

The main difference between proper XML and DSX is that you do not have to formally declare
your namespace in DSX. You can simply start using the namespace in your component tag names.

Formally, this means that DSX does not require the `xmlns` attribute to use a namespace.

*Notes:*
 - Namespaces are not required, but they are good etiquette if you plan on releasing your module to the world, or have a big team / project.
 - If you use a namespace, the render hook name will be prepended with your namespace. (@see next section for more details)

###Dynamic hooks
The recommended way for creating custom components in DSX is through the dynamic hook system.
As DSX parses your XML, if it encounters a custom tag, it will use `module_invoke_all()` to call a render hook that is derived from the name of the tag and the `_component` suffix.

 - The hook will receive the attributes of the tag as the first parameter and any containing inner text as the second parameter.
 - The hook *will not* receive any of the child elements in the parameters. This is for simplicity and composability. (It may change in the future.)
 - The hook is expected to return a Drupal render array:
    - The recommended workflow is to use DSX to generate your component.
    - You can use other custom DSX components when rendering your component, in fact it is encouraged! Composability is king.
    - You can generate the render array manually, which is useful for some advanced situations.

Example:
 - The tag `<my-custom-tag />` will call `HOOK_my_custom_tag_component($props, $value)`
 - Using namespaces: `<my-namespace:my-custom-tag />` will call `HOOK_my_namespace_my_custom_tag_component($props, $value)`

*Note:*
 - Each custom DSX component must have exactly *one* render function, no more, and no less.
 - If multiple render functions are defined, DSX will error and list all modules that implement competing render functions to help with debugging.
 - If no render function is defined, DSX will give you the format of the hook name that it was looking for.

 - You *can not* use a custom component inside it's own render function, or inside any component that the render function uses.
    - This would create an infinite recursive loop. Infinite loops are bad m'kay :stuck_out_tongue:
    - DSX actively checks for this, and will throw an error if it happens.
    - This kind of error can be subtle and hard to find, especially with many layers of nesting, so the Exception shows you the stack trace of render function calls to help you debug.

####The dsx-deep-embed attribute
The default behavior of DSX is to embed child elements directly underneath the first parent tag of a custom component, but you may want to nest child elements deeper for some component.

Example:
Say we have a `<table-elem />` defined like this:
```php
function dsx_table_elem_component($props, $value) {
  $attrs = drupal_attributes($props);
  return dsx_render(
  <<<DSX
<table id="test-elem" {$attrs}>
  <tbody>
    <tr>
      <td></td>
    </tr>
  </tbody>
</table>
DSX
  );
}
```

And we use the table element like this:
```php
function my_module_page() {
  return dsx_render(
<<<DSX
<table-elem><h2>inner content</h2></table-elem>
DSX
  );
}
```

The above page callback will produce a render array that creates HTML that looks like this:
```HTML
<table id="test-elem" {$attrs}>
  <h2>inner content</h2>
  <tbody>
    <tr>
      <td></td>
    </tr>
  </tbody>
</table>
```

Putting the `H2` tag underneath the `table` tag was probably not the intention of this component. To fix this, include the `dsx-deep-embed` attribute in the parent element: `<table id="test-elem" {$attrs} dsx-deep-embed="true">`

Full Example:
We have added the dsx-deep-embed to the table tag.
```php
function dsx_table_elem_component($props, $value) {
  $attrs = drupal_attributes($props);
  return dsx_render(
  <<<DSX
<table id="test-elem" {$attrs} dsx-deep-embed="true">
  <tbody>
    <tr>
      <td></td>
    </tr>
  </tbody>
</table>
DSX
  );
}
```

Now the page callback will produce a render array that looks like this, which is much better:
```HTML
<table id="test-elem" {$attrs}>
  <tbody>
    <tr>
      <td>
        <h2>inner content</h2>
      </td>
    </tr>
  </tbody>
</table>
```

*Notes:*
 - This feature is experimental. There may be a lot of possible edge cases that have not been addressed yet.
 - A custom component that uses this feature *must* have a single root node, and must only use simple nesting. There must be a clear path from the top of the tree to the bottom.
    - DSX checks this and will error out if the function does not comply.

###Traditional Drupal Method : hook_element_info()
DSX has full interop with the Drupal Form API through XML namespaces.
Adding a namespace to a tag will tell DSX to check Drupal `element_info()` for information about the tag before it attempts to call a custom render function.

 - the `drupal` namespace is not special in anyway. It's used by convention. (This may change in the future)

 - Just the fact that a namespace exists in the tag triggers the element_info() mechanism:
    - You could just as easily write `<crazyTalk:password>`, but why would you torture your fellow developers like that?
    - A more realistic use would be to add your own custom tag into the drupal namespace, which is fine, as long is it doesn't have the same name as another Drupal form element :stuck_out_tongue:

Example:
You would define the element like this:
```php
//Make the old skool way work, with namespaces.
function my_module_element_info() {
  return array(
    'customNS_test_elem' => array(
      '#input' => FALSE,
      '#theme' => 'customNS_test_elem' // This is important. You must indicate the render function.
    )
  );
}

function my_module_theme($existing, $type, $theme, $path) {
  return array(
    'customNS_test_elem' => array(
      'render element' => 'element',
    ),
  );
}

/* This is a traditional Drupal theme function, so it should return an HTML string, NOT a render array or DSX. */
function theme_customNS_test_elem($variables) {
  // Loop over element children, render them, and add them to the #value string.
  foreach (element_children($variables['element']) as $key) {
    if (!isset($variables['element']['#value'])) {
      // Set this to string to avoid E_NOTICE error when concatenating to NULL.
      $variables['element']['#value'] = '';
    }
    // Concatenate the rendered child onto the element #value.
    $variables['element']['#value'] .= drupal_render($variables['element'][$key]);
  }
  return '<div class="old-skool">' . $variables['element']['#value'] . '</div>';
}
```

You can then use the element like so:
```php
function my_module_page() {
  return dsx_render(
<<<DSX
<customNS:test-elem>stuff</customNS:test-elem>
DSX
  );
}
```

@see the Drupal API for more details:
 - `element_info()`: https://api.drupal.org/api/drupal/includes!common.inc/function/element_info/7
 - `hook_element_info()`: https://api.drupal.org/api/drupal/modules%21system%21system.api.php/function/hook_element_info/7


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
