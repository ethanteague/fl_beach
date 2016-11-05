<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>
<?php if ($logo): ?>
  <div id="logo-container"><a href="<?php print $front_page; ?>"
                              title="<?php print t('Home'); ?>" rel="home"
                              class="header__logo" id="logo"><img
        src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"
        class="header__logo-image"/></a></div>
<?php endif; ?>
<div id="above-top">
  <div id="at-wrap">
    <div id="at-left">
      Locally researched and approved vacation spots.
    </div>
    <div id="at-right">
      <a href="/featured-sponsors">Featured Sponsors</a> |
      <a href="/blog">Blog</a> |
      <a href="/contact">Contact Us</a>
      <div id="top-socials">
        <?php
        print '<a target="_blank" href="http://www.facebook.com/FloridaBeachTrails"><img src="/' . path_to_theme() . '/images/theme/twitter_small.png" /></a>';

        print '<a target="_blank" href="http://www.facebook.com/FloridaBeachTrails"><img src="/' . path_to_theme() . '/images/theme/facebook_small.png" /></a>';

        print '<a target="_blank" href="http://www.facebook.com/FloridaBeachTrails"><img src="/' . path_to_theme() . '/images/theme/pinterest_small.png" /></a>';
        ?>
      </div>
    </div>
  </div>
</div>
<div id="top">
  <div id="banner-verify">
    <?php
    if (drupal_is_front_page()) : ?>
      <a href="content/beach-verified"><img
          src="/' . path_to_theme() . '/images/theme/obv_banner.png"/></a>
    <?php endif; ?>
  </div>
  <div id="navigation">

    <?php if ($main_menu): ?>
      <nav id="main-menu" role="navigation" tabindex="-1">
        <?php
        // This code snippet is hard to modify. We recommend turning off the
        // "Main menu" on your sub-theme's settings form, deleting this PHP
        // code block, and, instead, using the "Menu block" module.
        // @see https://drupal.org/project/menu_block
        print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </nav>
    <?php endif; ?>

    <?php print render($page['navigation']); ?>

  </div>


</div>
<div id="wood"></div>
<?php if (drupal_is_front_page()): ?>
  <div id="slide-wrap">
    <div id="slide-frame">
      <?php print '<img src="/' . path_to_theme() . '/images/theme/frame.png" />'; ?>
    </div>
    <?php print render($page['highlighted']); ?>
  </div>
<? endif; ?>
<div id="page-wrap">
  <div id="page">

    <header class="header" id="header" role="banner">

      <?php if ($site_name || $site_slogan): ?>
        <div class="header__name-and-slogan" id="name-and-slogan">
          <?php if ($site_name): ?>
            <h1 class="header__site-name" id="site-name">
              <a href="<?php print $front_page; ?>"
                 title="<?php print t('Home'); ?>" class="header__site-link"
                 rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>

          <?php if ($site_slogan): ?>
            <div class="header__site-slogan"
                 id="site-slogan"><?php print $site_slogan; ?></div>
          <?php endif; ?>
        </div>
      <?php endif; ?>
      <?php endif; ?>
      <?php if ($secondary_menu): ?>
        <nav class="header__secondary-menu" id="secondary-menu"
             role="navigation">
          <?php print theme('links__system_secondary_menu', array(
            'links' => $secondary_menu,
            'attributes' => array(
              'class' => array('links', 'inline', 'clearfix'),
            ),
            'heading' => array(
              'text' => $secondary_menu_heading,
              'level' => 'h2',
              'class' => array('element-invisible'),
            ),
          )); ?>
        </nav>
      <?php endif; ?>

      <?php print render($page['header']); ?>

    </header>

    <div id="main">

      <div id="content" class="column" role="main">

        <?php print $breadcrumb; ?>
        <a id="main-content"></a>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 class="page__title title"
              id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print $messages; ?>
        <?php print render($tabs); ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php print render($page['content']); ?>
      </div>


      <?php
      // Render the sidebars to see if there's anything in them.
      $sidebar_first = render($page['sidebar_first']);
      $sidebar_second = render($page['sidebar_second']);
      ?>

      <?php if ($sidebar_first || $sidebar_second): ?>
        <aside class="sidebars">
          <?php print $sidebar_first; ?>
          <?php print $sidebar_second; ?>
        </aside>
      <?php endif; ?>

    </div>

    <?php print render($page['footer']); ?>

  </div>

  <?php print render($page['bottom']); ?>
</div>

<div id="below-foot-wrap">
  <div id="divider"></div>
  <div id="below-foot">
    <?php
    $menu = menu_navigation_links('main-menu');
    print theme('links__menu_main_menu', array('links' => $menu));

    ?>
    <div id="foot-socials">
      <span id="be-social">Be Social</span> <span class="social"><?php
        print '<a target="_blank" href="http://twitter.com/FLBeachTrails"><img src="/' . path_to_theme() . '/images/theme/twitter_big.png" /></a>';
        ?></span>

      <span class="social"><?php
        print '<a target="_blank" href="http://www.facebook.com/FloridaBeachTrails"><img src="/' . path_to_theme() . '/images/theme/facebook_big.png" /></a>';
        ?></span>

      <span class="social"><?php
        print '<a target="_blank" href="http://www.facebook.com/FloridaBeachTrails"><img src="/' . path_to_theme() . '/images/theme/pinterest_big.png" /></a>';
        ?></span>


    </div>
  </div>

</div>

<div id="bottom-page-wrap">

  <div id="bottom-page">
    <a class="reg" href="user/register">REGISTER YOUR BUSINESS ON FLORIDA BEACH
      TRAILS!</a> | ©Florida Beach Trails 2012
  </div>
</div>

