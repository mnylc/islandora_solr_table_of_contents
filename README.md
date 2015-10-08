# Islandora Solr Table of Contents [![Build Status](https://travis-ci.org/discoverygarden/islandora_solr_table_of_contents.png?branch=7.x)](https://travis-ci.org/discoverygarden/islandora_solr_table_of_contents)

## Introduction

The Islandora Solr Table of Contents module adds a block that provides a table of contents view of repository hierarchy via [jsTree](http://jstree.com). Children of individual objects lazy-load on demand through queries to Solr.

## Installation

Install as usual; check the [Drupal documentation](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

The [jsTree library](http://jstree.com) MUST be installed in your site's libraries folder. This module has been developed and tested against jsTree 3.0.0 and likely will not work with the most recent iteration.

## Configuration

Configuration options for the block exist at the site path admin/structure/block/manage/islandora_solr_table_of_contents/islandora_solr_table_of_contents/configure. These include:

- Enabling and specification of which content models to render the block on, as well as configuration to determine the Solr fields that store child relationships and how children are sorted. These fields should be string literal fields. By default, the block is only rendered on pages for object types that have children. This can be expanded to other content models that don't have children by checking off other content models and simply not filling out the "Solr Field" entry for that content model. This only really works with states enabled, however; otherwise when the object is determined to have no children, the block simply won't be rendered.
- The field Solr should use to get hasModel relationships; this is used to determine whether or not an object has children as configured using the above option. This can be a multivalued Solr field, and the block will get the first content model used by the object that has child relationships.
- The field Solr should use to get the label for objects to apply to the jsTree block.
- Whether or not to save the table of contents state for the duration of a user's session. If so, options are also given on where to draw the tree from.

## Troubleshooting/Issues

As this relies on sessionStorage to maintain states information about the tree, saving states won't work in Internet Explorer versions below 7.

As mentioned above, do not use this with versions of jsTree above 3.0.0; it has been tested against (and does not work with) jsTree 3.2.1, and likely will continue not to work with future iterations.

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)


## Maintainers/Sponsors
Current maintainers:

* [discoverygarden](https://github.com/discoverygarden)

This project has been sponsored by:

* [New York Historical Society](www.nyhistory.org)

## Development

If you would like to contribute to this module, please check out our helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the Islandora.ca site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
