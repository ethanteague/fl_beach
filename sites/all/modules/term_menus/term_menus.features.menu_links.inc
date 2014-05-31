<?php
/**
 * @file
 * term_menus.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function term_menus_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_beach-bars--dining:taxonomy/term/4
  $menu_links['main-menu_beach-bars--dining:taxonomy/term/4'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'taxonomy/term/4',
    'router_path' => 'taxonomy/term/%',
    'link_title' => 'Beach Bars & Dining',
    'options' => array(
      'attributes' => array(
        'title' => 'Beach Bars & Dining',
      ),
      'identifier' => 'main-menu_beach-bars--dining:taxonomy/term/4',
    ),
    'module' => 'taxonomy_menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 3,
    'customized' => 0,
  );
  // Exported menu link: main-menu_florida-beaches:taxonomy/term/1
  $menu_links['main-menu_florida-beaches:taxonomy/term/1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'taxonomy/term/1',
    'router_path' => 'taxonomy/term/%',
    'link_title' => 'Florida Beaches',
    'options' => array(
      'attributes' => array(
        'title' => 'Florida Beaches',
      ),
      'identifier' => 'main-menu_florida-beaches:taxonomy/term/1',
    ),
    'module' => 'taxonomy_menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 0,
    'customized' => 0,
  );
  // Exported menu link: main-menu_places-to-stay:taxonomy/term/2
  $menu_links['main-menu_places-to-stay:taxonomy/term/2'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'taxonomy/term/2',
    'router_path' => 'taxonomy/term/%',
    'link_title' => 'Places to Stay',
    'options' => array(
      'attributes' => array(
        'title' => 'Places to Stay',
      ),
      'identifier' => 'main-menu_places-to-stay:taxonomy/term/2',
    ),
    'module' => 'taxonomy_menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 1,
    'customized' => 0,
  );
  // Exported menu link: main-menu_things-to-do:taxonomy/term/3
  $menu_links['main-menu_things-to-do:taxonomy/term/3'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'taxonomy/term/3',
    'router_path' => 'taxonomy/term/%',
    'link_title' => 'Things To Do',
    'options' => array(
      'attributes' => array(
        'title' => 'Things To Do',
      ),
      'identifier' => 'main-menu_things-to-do:taxonomy/term/3',
    ),
    'module' => 'taxonomy_menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 2,
    'customized' => 0,
  );
  // Exported menu link: main-menu_vacation-planning:taxonomy/term/5
  $menu_links['main-menu_vacation-planning:taxonomy/term/5'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'taxonomy/term/5',
    'router_path' => 'taxonomy/term/%',
    'link_title' => 'Vacation Planning',
    'options' => array(
      'attributes' => array(
        'title' => 'Vacation Planning',
      ),
      'identifier' => 'main-menu_vacation-planning:taxonomy/term/5',
    ),
    'module' => 'taxonomy_menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'weight' => 4,
    'customized' => 0,
  );
  // Exported menu link: management_taxonomy-manager:admin/config/user-interface/taxonomy-manager-settings
  $menu_links['management_taxonomy-manager:admin/config/user-interface/taxonomy-manager-settings'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/config/user-interface/taxonomy-manager-settings',
    'router_path' => 'admin/config/user-interface/taxonomy-manager-settings',
    'link_title' => 'Taxonomy Manager',
    'options' => array(
      'attributes' => array(
        'title' => 'Advanced settings for the Taxonomy Manager',
      ),
      'identifier' => 'management_taxonomy-manager:admin/config/user-interface/taxonomy-manager-settings',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'parent_identifier' => 'management_user-interface:admin/config/user-interface',
  );
  // Exported menu link: management_taxonomy-manager:admin/structure/taxonomy_manager/voc
  $menu_links['management_taxonomy-manager:admin/structure/taxonomy_manager/voc'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/structure/taxonomy_manager/voc',
    'router_path' => 'admin/structure/taxonomy_manager/voc',
    'link_title' => 'Taxonomy Manager',
    'options' => array(
      'attributes' => array(
        'title' => 'Administer vocabularies with the Taxonomy Manager',
      ),
      'identifier' => 'management_taxonomy-manager:admin/structure/taxonomy_manager/voc',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'parent_identifier' => 'management_structure:admin/structure',
  );
  // Exported menu link: management_taxonomy:admin/structure/taxonomy
  $menu_links['management_taxonomy:admin/structure/taxonomy'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/structure/taxonomy',
    'router_path' => 'admin/structure/taxonomy',
    'link_title' => 'Taxonomy',
    'options' => array(
      'attributes' => array(
        'title' => 'Manage tagging, categorization, and classification of your content.',
      ),
      'identifier' => 'management_taxonomy:admin/structure/taxonomy',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 1,
    'expanded' => 0,
    'weight' => 0,
    'customized' => 0,
    'parent_identifier' => 'management_structure:admin/structure',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Beach Bars & Dining');
  t('Florida Beaches');
  t('Places to Stay');
  t('Taxonomy');
  t('Taxonomy Manager');
  t('Things To Do');
  t('Vacation Planning');


  return $menu_links;
}
