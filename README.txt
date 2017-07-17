CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Site map term path (and Pathauto)
 * Maintainers


INTRODUCTION
------------

Site Map provides a site map that gives visitors an overview of your site.
It can also display the RSS feeds for all blogs and terms. Drupal generates the
RSS feeds automatically but few seems to be aware that they exist.

The site map can display the following items:

 * A message to be displayed above the site map
 * The front page.
 * The latest blogs.
 * Any books that optionally will be displayed fully expanded.
 * Any menus that will be displayed fully expanded.
 * Any vocabulary with all the terms expanded.
   Optionally with node counts and RSS feeds.
 * Display comment RSS links if the Comment RSS module is installed.
 * A syndication block, the "more" link goes to the site map.


REQUIREMENTS
------------

No special requirements


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7 for further
information.


CONFIGURATION
-------------

 1. Have a look at the different settings in
   Administer » Site configuration » Site map

 2. Visit http://example.com/sitemap.

If you have many books/menus/vocabularies the Checkall module is recommended.
It will implement "Check all / Uncheck all" for checkboxes on the site map
settings page. http://drupal.org/project/checkall


SITE MAP TERM PATH (AND PATHAUTO)
-------------------------------
There is a "depth" setting on the Site map settings page where you can adjust
how site map constructs the term links.

For making Site map build the same path that Pathauto per default generates
alias for you should set this to "-1" I believe.


MAINTAINERS
-----------

Author - Fredrik Jonsson fredrik at combonet dot se
License - GPL (see LICENSE)

Current maintainers:
 * Fredrik Jonsson (frjo) - https://www.drupal.org/user/5546
 * Renato Gonçalves (RenatoG) - https://www.drupal.org/user/3326031
 * Nik Alexandrov (Nafes) - https://www.drupal.org/user/2489926
 * Amit Goyal (amitgoyal) - https://www.drupal.org/user/56441
 * Colan Schwartz (colan) - https://www.drupal.org/user/58704
 * Nitin Goyal (daffodilsoftware) - https://www.drupal.org/user/1050564
 * Darrell Ulm (darrell_ulm) - https://www.drupal.org/user/46284
 * Pavlo Samchuk (id.medion) - https://www.drupal.org/user/2542592
 * john.oltman - https://www.drupal.org/user/699926
 * Meenu Gupta (meenugupta) - https://www.drupal.org/user/265769
 * Vijay Thummar (vijaythummar) - https://www.drupal.org/user/119244
 * vlaad.bo - https://www.drupal.org/user/2696261
