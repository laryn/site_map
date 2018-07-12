<?php
/**
 * @file
 * Admin functions.
 */

/**
 * Menu callback; presents the sitemap settings page.
 */
function site_map_admin_settings_form() {
  $config = config('site_map.settings');
  $form = array('#config' => 'site_map.settings');

  $form['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page settings'),
  );

  $abs_base_path = url(base_path(), array('absolute' => TRUE));
  $site_map_path = $config->get('path');
  $form['page']['intro'] = array(
    '#markup' => t(
      'All settings below will apply to the <a href="@link">site map page</a>.',
      array('@link' => url($site_map_path))
    ),
  );
  // Field to set site map page path.
  $form['page']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Site map page URL'),
    '#default_value' => $config->get('path'),
    '#field_prefix' => $abs_base_path,
    '#size' => 25,
  );
  // Field to set site map page title.
  $form['page']['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page title'),
    '#default_value' => $config->get('page_title'),
    '#description' => t(
      'Leave blank to use the default value of <em>Site map</em>.',
      array('@link' => url($site_map_path))
    ),
    '#attributes' => array('placeholder' => 'Site map'),
  );
  $form['page']['css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove site map styles.'),
    '#default_value' => $config->get('css'),
  );
  // Field to set site map message.
  $site_map_message = $config->get('message');
  $form['page']['message'] = array(
    '#type' => 'text_format',
    '#format' => isset($site_map_message['format']) ? $site_map_message['format'] : NULL,
    '#title' => t('Message to be displayed above the site map'),
    '#default_value' => $site_map_message['value'],
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site map content'),
  );
  $site_map_ordering = array();

  $form['content']['show_front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the front page.'),
    '#default_value' => $config->get('show_front'),
  );
  $site_map_ordering['front'] = t('Front page');

  $form['content']['show_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include section titles.'),
    '#default_value' => $config->get('show_titles'),
  );

  $form['content']['show_node_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use full page titles instead of menu link titles.'),
    '#default_value' => $config->get('show_node_titles'),
  );

  $menu_options = menu_get_menus();
  $menu_defaults = $config->get('show_menus');
  foreach ($menu_options as $menu_id => $menu_title) {
    if ($menu_defaults[$menu_id]) {
      $site_map_ordering['menus_' . $menu_id] = $menu_title;
    }
  }
  $form['content']['show_menus'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus to include in the site map:'),
    '#default_value' => $menu_defaults,
    '#options' => $menu_options,
    '#multiple' => TRUE,
  );

  $form['content']['show_menus_hidden'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show disabled or hidden menu items'),
    '#default_value' => $config->get('show_menus_hidden'),
  );

  $vocab_options = array();
  $vocab_defaults = $config->get('show_vocabularies');
  if (module_exists('taxonomy')) {
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $vocab_options[$vocabulary->machine_name] = $vocabulary->name;
      if ($vocab_defaults[$vocabulary->machine_name]) {
        $site_map_ordering['vocabularies_' . $vocabulary->machine_name] = $vocabulary->name;
      }
    }
  }
  $form['content']['show_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Categories (taxonomy terms) to include in the site map:'),
    '#default_value' => $vocab_defaults,
    '#options' => $vocab_options,
    '#multiple' => TRUE,
  );

  if (module_exists('book')) {
    $book_options = array();
    foreach (book_get_books() as $book) {
      $book_options[$book['mlid']] = $book['title'];
    }
    $form['content']['show_books'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Books to include in the site map'),
      '#default_value' => $config->get('show_books'),
      '#options' => $book_options,
      '#multiple' => TRUE,
    );
    $form['content']['books_expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show all children pages for each book.'),
      '#default_value' => $config->get('books_expanded'),
    );
    $site_map_ordering['books'] = t('Books');
  }

  if (module_exists('faq')) {
    $form['content']['show_faq'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show FAQ module content'),
      '#default_value' => $config->get('show_faq'),
    );
    $site_map_ordering['faq'] = t('FAQ content');
  }

  if (module_exists('blog')) {
    $form['content']['show_blogs'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show the 10 most active blog authors'),
      '#default_value' => $config->get('show_blogs'),
    );
    $site_map_ordering['blogs'] = t('Active blog authors');
  }

  $form['content']['order'] = array(
    '#type' => 'item',
    '#title' => t('Site map order'),
    '#theme' => 'site_map_order',
  );

  $site_map_order_defaults = $config->get('order');
  foreach ($site_map_ordering as $content_id => $content_title) {
    $weight = isset($site_map_order_defaults[$content_id]) ? $site_map_order_defaults[$content_id] : -50;
    $form['content']['order'][$content_id] = array(
      'content' => array(
        '#markup' => $content_title,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight for @title', array('@title' => $content_title)),
        '#title_display' => 'invisible',
        '#delta' => 50,
        '#default_value' => $weight,
        '#parents' => array('order', $content_id),
      ),
      '#weight' => $weight,
    );
  }
  $form['#attached']['js'][] = backdrop_get_path('module', 'site_map') . '/js/site_map.admin.js';

  $form['taxonomy_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Categories settings'),
  );
  $form['taxonomy_options']['show_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the taxonomy term description.'),
    '#default_value' => $config->get('show_description'),
  );
  $form['taxonomy_options']['show_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the number of items for each taxonomy term.'),
    '#default_value' => $config->get('show_count'),
  );
  $form['taxonomy_options']['link_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create links for empty categories.'),
    '#default_value' => $config->get('link_empty'),
  );
  $form['taxonomy_options']['categories_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('How many levels of subcategories should be included?'),
    '#default_value' => $config->get('term_depth'),
    '#size' => 3,
    '#maxlength' => 10,
    '#description' => t('Enter a positive number, or "all" to include all levels.'),
  );
  $form['taxonomy_options']['term_threshold'] = array(
    '#type' => 'number',
    '#title' => t('Term count threshold'),
    '#default_value' => $config->get('term_threshold'),
    '#min' => -1,
    '#description' => t('Only taxonomy terms whose node counts are greater than this threshold will be shown. Set to -1 to disable.'),
  );
  $form['taxonomy_options']['forum_threshold'] = array(
    '#type' => 'number',
    '#title' => t('Forum count threshold'),
    '#default_value' => $config->get('forum_threshold'),
    '#min' => -1,
    '#description' => t('Only forums whose node counts are greater than this threshold will be shown. Set to -1 to disable.'),
  );

  $form['rss_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS settings'),
  );
  $form['rss_options']['show_rss_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include links to RSS feeds'),
    '#default_value' => $config->get('show_rss_links'),
  );
  $form['rss_options']['rss_front'] = array(
    '#type' => 'textfield',
    '#title' => t('URL for home page RSS feed'),
    '#default_value' => $config->get('rss_front'),
    '#field_prefix' => $abs_base_path,
    '#attributes' => array('placeholder' => 'rss.xml'),
  );

  // Make use of the Checkall module if it's installed.
  if (module_exists('checkall')) {
    $form['content']['show_books']['#checkall'] = TRUE;
    $form['content']['show_menus']['#checkall'] = TRUE;
    $form['content']['show_vocabularies']['#checkall'] = TRUE;
  }

  // Add our own submit handler.
  $form['#submit'][] = 'site_map_admin_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Custom submit handler for the Site Map Admin Settings form.
 */
function site_map_admin_settings_form_submit($form, &$form_state) {
  // Since we allow the user to choose the path of their sitemap, we need to
  // refresh router entries.
  menu_rebuild();
}
