<?php

/**
 * @file
 * Theming functionality for table of contents.
 */

/**
 * Theme the block management table.
 *
 * @param array $variables
 *   Theme variables passed in.
 *
 * @return string
 *   Table markup.
 */
function theme_islandora_solr_table_of_contents_block_management_table(array $variables) {
  drupal_add_js('misc/tableselect.js');
  $row_elements = $variables['table']['rows'];
  $rows = array();
  foreach (element_children($row_elements) as $key) {
    $columns = array();
    $row_element = $row_elements[$key];
    foreach (element_children($row_element) as $key) {
      $column_element = $row_element[$key];
      $class = "table_of_contents_block_management_{$key}";
      $columns[] = array(
        'data' => drupal_render($column_element),
        'class' => isset($cell_element['#attributes']['class']) ? $column_element['#attributes']['class'] : $class,
      );
    }
    $rows[] = $columns;
  }
  $variables = array(
    'header' => $variables['table']['#header'],
    'rows' => $rows,
    'attributes' => $variables['table']['#attributes'],
    'caption' => NULL,
    'colgroups' => NULL,
    'sticky' => NULL,
    'empty' => t("No content models."),
  );
  return theme('table', $variables);
}
