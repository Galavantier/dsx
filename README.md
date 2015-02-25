Dsx
===

Inspired by Facebooks' JSX and XHP projects,
The module takes XML/HTML strings and converts them into Drupal Render Arrays.
This lets Drupal modules define xml directly instead of using templates.
Allowing for much more modular designs, i.e.
 - Atomic design by Brad Frost (http://bradfrost.com/blog/post/atomic-web-design/),
 - or Google's card based design.

DSX supports:
 - XML namespaces,
 - The full Drupal form API,
 - Is extensible via dynamic hooks for custom XML tags,
 - Or via the more traditional hook_element_info().

Installation
==============
Install and Enable the module in Drupal.

Usage
======
Any time you would call theme('template_name'), you can instead call dsx_render(<DSX String>).

Defining Custom Tags
====================

Namespaces
===========

Dynamic hooks
=============

Traditional Drupal Method hook_element_info()
==============================================
