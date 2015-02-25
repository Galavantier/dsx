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
 - The full Drupal form API,
 - Is extensible via dynamic hooks for custom XML tags,
 - Or via the more traditional hook_element_info().

Installation
==============
Install and Enable the module in Drupal.

There is no magic here; no hacking of Drupal core or custom PHP extensions. :-)

Usage
======
Any time you would return themed output,
use a Drupal Render Array,
or call `theme('template_name')`,
you can instead call `dsx_render(<DSX String>)`.

`dsx_render()` takes an xml string as input, and produces a Drupal Render Array as output.
All text is sanitized and escaped using Drupal's built in sanitation functions before added to the render array.

##PHP Heredoc Syntax


Defining Custom Tags
====================

##Namespaces


##Dynamic hooks


##Traditional Drupal Method : hook_element_info()
