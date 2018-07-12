<?php
/**
 * @file
 * Admin functions.
 */

/**
 * Menu callback; presents the sitemap settings page.
 */
function site_map_admin_settings_form() {
  $abs_base_path = url(base_path(), array('absolute' => TRUE));
  $site_map_path = variable_get('site_map_path', 'sitemap');

  $form['intro'] = array(
    '#markup' => t(
      'All settings below will apply to the <a href="@link">site map page</a>.',
      array('@link' => url($site_map_path))
    ),
  );

  // Field to set site map page path.
  $form['site_map_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Site map page URL'),
    '#default_value' => variable_get('site_map_path', 'sitemap'),
    '#field_prefix' => $abs_base_path,
    '#size' => 25,
  );

  // Field to set site map page title.
  $form['site_map_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => variable_get('site_map_page_title', FALSE),
    '#description' => t(
      'Leave blank to use the default value of <em>Site map</em>.',
      array('@link' => url($site_map_path))
    ),
    '#attributes' => array('placeholder' => 'Site map'),
  );

  $form['site_map_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove site map styles.'),
    '#default_value' => variable_get('site_map_css', 0),
  );

  // Field to set site map message.
  $site_map_message = variable_get('site_map_message', array('value' => '', 'format' => NULL));
  $form['site_map_message'] = array(
    '#type' => 'text_format',
    '#format' => isset($site_map_message['format']) ? $site_map_message['format'] : NULL,
    '#title' => t('Message to be displayed above the site map'),
    '#default_value' => $site_map_message['value'],
  );

  $form['site_map_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site map content'),
  );
  $site_map_ordering = array();

  $form['site_map_content']['site_map_show_front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the front page.'),
    '#default_value' => variable_get('site_map_show_front', 1),
  );
  $site_map_ordering['front'] = t('Front page');

  $form['site_map_content']['site_map_show_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include section titles.'),
    '#default_value' => variable_get('site_map_show_titles', 1),
  );

  $form['site_map_content']['site_map_show_node_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use full page titles instead of menu link titles.'),
    '#default_value' => variable_get('site_map_show_node_titles'),
  );

  $menu_options = menu_get_menus();
  foreach ($menu_options as $menu_id => $menu_title) {
    $site_map_ordering['menus_' . $menu_id] = $menu_title;
  }
  $form['site_map_content']['site_map_show_menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus to include in the site map:'),
    '#default_value' => variable_get('site_map_show_menus', array()),
    '#options' => $menu_options,
    '#multiple' => TRUE,
  );

  $form['site_map_content']['site_map_show_menus_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show disabled or hidden menu items'),
    '#default_value' => variable_get('site_map_show_menus_hidden', 0),
  );

  if (module_exists('faq')) {
    $form['site_map_content']['site_map_show_faq'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show FAQ module content'),
      '#default_value' => variable_get('site_map_show_faq', 0),
    );
    $site_map_ordering['faq'] = t('FAQ content');
  }

  $vocab_options = array();
  if (module_exists('taxonomy')) {
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $vocab_options[$vocabulary->machine_name] = $vocabulary->name;
      $site_map_ordering['vocabularies_' . $vocabulary->machine_name] = $vocabulary->name;
    }
  }
  $form['site_map_content']['site_map_show_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Categories (taxonomy terms) to include in the site map:'),
    '#default_value' => variable_get('site_map_show_vocabularies', array()),
    '#options' => $vocab_options,
    '#multiple' => TRUE,
  );

  if (module_exists('book')) {
    $book_options = array();
    foreach (book_get_books() as $book) {
      $book_options[$book['mlid']] = $book['title'];
    }
    $form['site_map_content']['site_map_show_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Books to include in the site map'),
      '#default_value' => variable_get('site_map_show_books', array()),
      '#options' => $book_options,
      '#multiple' => TRUE,
    );
    $form['site_map_content']['site_map_books_expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show all children pages for each book.'),
      '#default_value' => variable_get('site_map_books_expanded', 1),
    );
    $site_map_ordering['books'] = t('Books');
  }

  // @todo remove now that blog is contrib?
  if (module_exists('blog')) {
    $form['site_map_content']['site_map_show_blogs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show the 10 most active blog authors'),
      '#default_value' => variable_get('site_map_show_blogs', 1),
    );
    $site_map_ordering['blogs'] = t('Active blog authors');
  }

  $form['site_map_content']['site_map_order'] = array(
    '#type' => 'item',
    '#title' => t('Site map order'),
    '#theme' => 'site_map_order',
  );
  $site_map_order_defaults = variable_get('site_map_order', array());
  foreach ($site_map_ordering as $content_id => $content_title) {
    $form['site_map_content']['site_map_order'][$content_id] = array(
      'content' => array(
        '#markup' => $content_title,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight for @title', array('@title' => $content_title)),
        '#title_display' => 'invisible',
        '#delta' => 50,
        '#default_value' => isset($site_map_order_defaults[$content_id]) ? $site_map_order_defaults[$content_id] : -50,
        '#parents' => array('site_map_order', $content_id),
      ),
      '#weight' => isset($site_map_order_defaults[$content_id]) ? $site_map_order_defaults[$content_id] : -50,
    );
  }
  $form['#attached']['js'][] = backdrop_get_path('module', 'site_map') . '/js/site_map.admin.js';

  $form['site_map_taxonomy_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Categories settings'),
  );
  $form['site_map_taxonomy_options']['site_map_show_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the taxonomy term description.'),
    '#default_value' => variable_get('site_map_show_description', 1),
  );
  $form['site_map_taxonomy_options']['site_map_show_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the number of nodes for each taxonomy term.'),
    '#default_value' => variable_get('site_map_show_count', 1),
  );
  $form['site_map_taxonomy_options']['site_map_link_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create links for empty categories.'),
    '#default_value' => variable_get('site_map_link_empty', 0),
  );
  $form['site_map_taxonomy_options']['site_map_categories_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('How many levels of subcategories should be included?'),
    '#default_value' => variable_get('site_map_categories_depth', 'all'),
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Enter a positive number, or "all" to include all levels.'),
  );
  $form['site_map_taxonomy_options']['site_map_term_threshold'] = array(
    '#type' => 'number',
    '#title' => t('Term count threshold'),
    '#default_value' => variable_get('site_map_term_threshold', 0),
    '#min' => -1,
    '#description' => t('Only taxonomy terms whose node counts are greater than this threshold will be shown. Set to -1 to disable.'),
  );
  $form['site_map_taxonomy_options']['site_map_forum_threshold'] = array(
    '#type' => 'number',
    '#title' => t('Forum count threshold'),
    '#default_value' => variable_get('site_map_forum_threshold', -1),
    '#min' => -1,
    '#description' => t('Only forums whose node counts are greater than this threshold will be shown. Set to -1 to disable.'),
  );

  $form['site_map_rss_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS settings'),
  );
  $form['site_map_rss_options']['site_map_show_rss_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include links to RSS feeds'),
    '#default_value' => variable_get('site_map_show_rss_links', 1),
  );
  $form['site_map_rss_options']['site_map_rss_front'] = array(
    '#type' => 'textfield',
    '#title' => t('URL for home page RSS feed'),
    '#default_value' => variable_get('site_map_rss_front', 'rss.xml'),
    '#field_prefix' => $abs_base_path,
    '#attributes' => array('placeholder' => 'rss.xml'),
  );
  $form['site_map_rss_options']['site_map_rss_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('How many subcategories should be included in the RSS feed?'),
    '#default_value' => variable_get('site_map_rss_depth', 'all'),
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Enter a positive number, "all" to include all subcategories, or "0" to include no subcategories.'),
  );

  // Make use of the Checkall module if it's installed.
  if (module_exists('checkall')) {
    $form['site_map_content']['site_map_show_books']['#checkall'] = TRUE;
    $form['site_map_content']['site_map_show_menus']['#checkall'] = TRUE;
    $form['site_map_content']['site_map_show_vocabularies']['#checkall'] = TRUE;
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'site_map_admin_settings_form_submit';

  return $form;
}

/**
 * Custom submit handler for the Site Map Admin Settings form.
 */
function site_map_admin_settings_form_submit($form, &$form_state) {
  // Since we allow the user to choose the path of their sitemap, we need to
  // refresh router entries.
  menu_rebuild();
}
