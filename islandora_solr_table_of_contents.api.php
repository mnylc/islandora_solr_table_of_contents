<?php

/**
 * @file
 * Describes hooks for islandora_solr_table_of_contents.
 */

/**
 * Provides for default table of contents configuration.
 *
 * The configuration is an array whose keys are content model PIDs associated
 * with arrays, which can contain:
 * - nothing: in this case, the Table of Contents block will be rendered on
 *   object pages with this content model, but they are not considered to have
 *   children.
 * - 'solr_field': a field in Solr designating how children relate to objects
 *   with this content model (e.g., RELS_EXT_isMemberOfCollection_uri_ms would
 *   be used to find children of an islandora:collectionCModel object). This can
 *   be a field that returns a PID, or a field that returns an info:fedora URI.
 * - 'sort_field': an optional field to sort Solr results on. This won't do
 *   anything by itself.
 *
 * @return array
 *   Default configuration settings array.
 */
function hook_islandora_solr_table_of_contents_default_configuration() {
  $configuration = array();
  // Add in a content model that renders the block but has no children.
  $configuration['some:contentModel'] = array();
  // Add in a content model that has children.
  $configuration['another:contentModel'] = array(
    'solr_field' => 'some_solr_field',
  );
  // Add in a content model that has children that are sorted.
  $configuration['some:sortedCModel'] = array(
    'solr_field' => 'some_other_solr_field',
    'sort_field' => 'some_sortable_field',
  );
  return $configuration;
}

/**
 * Alteration hook for the table of contents default configuration.
 *
 * @param array $configuration
 *   The default configuration array, passed in by reference.
 */
function hook_islandora_solr_table_of_contents_default_configuration_alter(&$configuration) {
  $configuration['some:existingCModel']['solr_field'] = 'a_different_field_than_the_one_originally_configured';
}
