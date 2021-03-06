<?php
/**
 * @file
 * Site map theme functions.
 */

/**
 * Returns HTML for a site map feed icon legend.
 */
function theme_site_map_rss_legend() {
  $output = '<p><strong>' . t('Legend:') . '</strong><br />';
  $output .= '<span class="rss">' . theme('site_map_feed_icon', array('type' => 'node')) . '</span> ' . t('Link to a content RSS feed');
  $output .= '<br /><span class="rss">' . theme('site_map_feed_icon', array('type' => 'comment')) . '</span> ' . t('Link to a comment RSS feed');
  $output .= '</p>';

  return $output;
}

/**
 * Prepares variables for theme_site_map_box().
 */
function template_preprocess_site_map_box(&$variables) {
  $variables['attributes']['class'][] = 'site-map-box';
}

/**
 * Returns HTML for a themed site map box.
 *
 * @param array $variables
 *   An associative array containing:
 *   - title: The subject of the box.
 *   - content: The content of the box.
 *   - attributes:  Optional attributes for the box.
 *
 * @return string
 *   Returns sitemap display in DIV.
 */
function theme_site_map_box($variables) {
  $title = $variables['title'];
  $content = $variables['content'];
  $attributes = $variables['attributes'];
  $options = $variables['options'];

  $output = '';
  if (!empty($title) || !empty($content)) {
    $output .= '<div' . backdrop_attributes($attributes) . '>';
    if (!empty($title) && isset($options['show_titles'])) {
      $output .= '<h2 class="title">' . $title . '</h2>';
    }
    if (!empty($content)) {
      $output .= '<div class="content">' . $content . '</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

/**
 * Returns HTML for a feed icon with link.
 *
 * @param array $variables
 *   An associative array containing:
 *   - url: The url of the feed.
 *   - name: The name of the feed.
 *   - type: The type of feed icon.
 *
 * @return string
 *   Constructs and returns html with feed image icon.
 */
function theme_site_map_feed_icon($variables) {
  $output = '';

  switch ($variables['type']) {
    case 'node':
      $output = theme('image', array(
        'path' => backdrop_get_path('module', 'site_map') . '/images/feed-small.png',
        'alt' => t('Syndicated feed icon'),
      )
      );
      break;

    case 'comment':
      $output = theme('image', array(
        'path' => backdrop_get_path('module', 'site_map') . '/images/feed-small-comment.png',
        'alt' => t('Syndicated feed icon'),
      )
      );
      break;
  }

  if (!empty($variables['url'])) {
    $output = l($output, $variables['url'], array(
      'attributes' => array(
        'class' => array('feed-link'),
        'title' => t('Syndicated feed for') . ' ' . $variables['name'],
      ),
      'html' => TRUE,
    ));
  }

  return $output;
}

/**
 * Prepares variables for theme_site_map_menu_tree().
 *
 * This is a clone of the core template_preprocess_menu_tree() function
 * with the exception of the site_map specific class name used in the
 * UL that also allow themers to override the function only
 * for the site map page.
 */
function template_preprocess_site_map_menu_tree(&$variables) {
  $variables['tree'] = $variables['tree']['#children'];
}

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * This is a clone of the core theme_menu_tree() function with the exception of
 * the site_map specific class name used in the UL that also allow themers to
 * override the function only for the site map page.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @return string
 *   Returns the html string with the <ul> for the menu tree.
 *
 * @see template_preprocess_menu_tree()
 * @ingroup themeable
 */
function theme_site_map_menu_tree($variables) {
  return '<ul class="site-map-menu">' . $variables['tree'] . '</ul>';
}

/**
 * Returns HTML for a menu link and submenu.
 *
 * This is a one by one clone of the core theme_menu_link() function that allows
 * custom theming of the site map page items.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   Returns html string for menu link.
 *
 * @ingroup themeable
 */
function theme_site_map_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = backdrop_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . backdrop_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Prepares variables for site map templates.
 *
 * @see site-map.tpl.php
 */
function template_preprocess_site_map(&$variables) {
  $config = config('site_map.settings');
  $message = $config->get('message');
  if (!empty($message)) {
    $variables['message'] = check_markup($message['value'], $message['format']);
  }

  if (($config->get('show_rss_links', 1) != 0) && module_exists('commentrss')
      && config_get('commentrss.settings', 'commentrss_site', COMMENTRSS_SITE_FRONT_PAGE)) {
    $variables['rss_legend'] = theme('site_map_rss_legend');
  }

  $variables['show_titles'] = $config->get('show_titles');
  $variables['site_map'] = '';
  $site_map_order = $config->get('order');
  asort($site_map_order);

  foreach ($site_map_order as $content => $weight) {
    // Get type of content.
    $type = substr($content, 0, strpos($content, '_'));
    $id = substr($content, strpos($content, '_') + 1);
    if (empty($type)) {
      $type = $content;
      $id = NULL;
    }

    switch ($type) {
      case 'front':
        if ($config->get('show_front')) {
          $variables['site_map'] .= _site_map_front_page();
        }
        break;

      case 'blogs':
        if ($config->get('show_blogs')) {
          $variables['site_map'] .= _site_map_blogs();
        }
        break;

      case 'books':
        $books = $config->get('show_books');
        if (!empty($books)) {
          $variables['site_map'] .= _site_map_books();
        }
        break;

      case 'menus':
        // Get the list of menus we'll be displaying.
        $menus = $config->get('show_menus');
        // Allow other modules to alter it.
        backdrop_alter('site_map_menu_list', $menus);
        if (!empty($menus[$id])) {
          $variables['site_map'] .= _site_map_menus($id);
        }
        break;

      case 'faq':
        if ($config->get('show_faq')) {
          $variables['site_map'] .= _site_map_faq();
        }
        break;

      case 'vocabularies':
        $vocabulary = taxonomy_vocabulary_machine_name_load($id);
        $vocabularies = $config->get('show_vocabularies');
        if (!empty($vocabularies[$vocabulary->machine_name])) {
          $variables['site_map'] .= _site_map_taxonomys($vocabulary);
        }
        break;
    }
  }

  // Invoke all custom modules and integrate themed HTML into the site map.
  $additional = module_invoke_all('site_map');
  foreach ($additional as $themed_site_map_code) {
    $variables['additional'] .= $themed_site_map_code;
  }
}

/**
 * Returns HTML for the site map order form.
 *
 * Copied from the core theme_filter_admin_format_filter_order() function.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_site_map_order($variables) {
  $element = $variables['element'];

  // Site map order (tabledrag).
  $rows = array();
  foreach (element_children($element, TRUE) as $name) {
    $element[$name]['weight']['#attributes']['class'][] = 'site-map-order-weight';
    $rows[] = array(
      'data' => array(
        backdrop_render($element[$name]['content']),
        backdrop_render($element[$name]['weight']),
      ),
      'class' => array('draggable'),
    );
  }
  $output = backdrop_render_children($element);
  $output .= theme('table', array(
    'rows' => $rows,
    'attributes' => array('id' => 'site-map-order'),
  ));
  backdrop_add_tabledrag('site-map-order', 'order', 'sibling', 'site-map-order-weight', NULL, NULL, TRUE);

  return $output;
}
