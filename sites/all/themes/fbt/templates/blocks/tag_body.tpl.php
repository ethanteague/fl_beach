<?php
$term = is_numeric(arg(2)) ? taxonomy_term_load(arg(2)) : '';
?>
<div id="content-wrap">
  <div id="top-wrap">
<?
  print !empty($term->description) ? $term->description : '';
if ($term->tid == 276) {
  print '<div style="clear:both"></div>';
  $map_all_view = module_invoke('views', 'block_view', 'map_upload-block_1');
  if (!empty($map_all_view['content']['#markup'])){

    print '<div id="map-wrap">'.$map_all_view['content']['#markup'].'</div>';
  }
}

if ($term->tid == 5) {
  print '<div style="clear:both"></div>';
  $map_all_view = module_invoke('views', 'block_view', 'itinerary-block');
  if (!empty($map_all_view['content']['#markup'])){

    print '<div id="map-all-wrap">'.$map_all_view['content']['#markup'].'</div>';
  }
}
?>
</div>


<?php

?>
<div id="left-wrap" class="custom-div no-line">
  <div class="callout">
<?php

  if ($term->tid != 276 && $term->tid != 5) {
    if ($term->tid == 1) {
?>
<h3>Pick Your Beach</h3>
<?php
    }
    else {
?>
      <h3>Listings</h3>

<?php
    }


?>
<?php
  $callout_view = module_invoke('views', 'block_view', 'related_content-block');
    print $callout_view['content']['#markup'];
  }
?>
  </div>

  </div>
  <div id="middle-wrap" class="custom-div">
<?php

  if ($term->tid != 276 && $term->tid != 5){

  $map_view = module_invoke('views', 'block_view', 'business_map-block_2');
print '<h3>Explore Locations</h3>';
  print $map_view['content']['#markup'];
  }
?>
</div>

</div>

<div id="right-wrap">
<?php
  if(!empty($term->field_ads_for_this_page[LANGUAGE_NONE])){
    $fcs = $term->field_ads_for_this_page[LANGUAGE_NONE];

    foreach ($fcs as $fc) {
      $full_fcs = entity_load('field_collection_item',$fc);
      foreach($full_fcs as $full_fc){
        $image_path = $full_fc->field_ad_image[LANGUAGE_NONE][0]['uri'];
        $image = image_style_url('ads',$image_path);
        print l('<img src="'.$image.'" />', $full_fc->field_external_link[LANGUAGE_NONE][0]['value'], array('html' => true, 'attributes' => array('target'=>'_blank')));
      }
    }
  }
?>


</div>
<div style="clear:both">
<?php
  print !empty($term->field_bottom_body_field[LANGUAGE_NONE][0]['safe_value']) ? $term->field_bottom_body_field[LANGUAGE_NONE][0]['safe_value'] : '';
?>
  </div>
