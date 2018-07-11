<?php
/**
 * @file
 * Theme implementation to display the site map.
 *
 * Available variables:
 * - $message:
 * - $rss_legend:
 * - $front_page:
 * - $blogs:
 * - $books:
 * - $menus:
 * - $faq:
 * - $taxonomys:
 * - $additional:
 * - $classes:
 *
 * @see template_preprocess()
 * @see template_preprocess_site_map()
 */
?>
<div class="site-map <?php print implode(' ', $classes); ?>">
  <?php if ($message): ?>
    <div class="site-map-message">
      <?php print $message; ?>
    </div>
  <?php endif; ?>

  <?php if ($rss_legend): ?>
    <div class="site-map-rss-legend">
      <?php print $rss_legend; ?>
    </div>
  <?php endif; ?>

  <?php print $site_map; ?>

  <?php if ($additional): ?>
    <div class="site-map-additional">
      <?php print $additional; ?>
    </div>
  <?php endif; ?>
</div>
