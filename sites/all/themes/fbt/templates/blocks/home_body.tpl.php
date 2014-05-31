<?php
$nid = node_load(arg(1));

   ?>
<div id="content-wrap">
  <div id="top-wrap">
    <?
       print $nid->body[LANGUAGE_NONE][0]['safe_value'];
    ?>
  </div>
  <div id="left-wrap" class="custom-div">
    <div class="callout">
      <h3>On the Beach Verified</h3>
      <?php
	 print $nid->field_callout_box_top[LANGUAGE_NONE][0]['safe_value'];
      ?>
    </div>
    <div class="callout bottom">
      <h3><span class="red">Hot</span> New FREE Item!</h3>
      <?php
  print $nid->field_callout_box_bottom[LANGUAGE_NONE][0]['safe_value'];
       $callout_view = module_invoke('views', 'block_view', 'callout_block_images-block');
print $callout_view['content']['#markup'];
      ?>
    </div>
  </div>
  <div id="middle-wrap" class="custom-div">
    <?php
       $map_view = module_invoke('views', 'block_view', 'business_map-block_1');
       print '<h3>Find a Florida Beach</h3>';
       print $map_view['content']['#markup'];
       ?>
</div>

</div>
<div id="right-wrap">
    <?php
  $fcs = $nid->field_ads_for_this_page[LANGUAGE_NONE];

foreach ($fcs as $fc) {
  $full_fcs = entity_load('field_collection_item',$fc);
  foreach($full_fcs as $full_fc){
    $image_path = $full_fc->field_ad_image[LANGUAGE_NONE][0]['uri'];
    $image = image_style_url('ads',$image_path);
    print l('<img src="'.$image.'" />', $full_fc->field_external_link[LANGUAGE_NONE][0]['value'], array('html' => true, 'attributes' => array('target'=>'_blank')));
  }
}
       ?>
</div>
<div style="clear:both"></div>
