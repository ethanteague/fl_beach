<?php
if (is_numeric(arg(1))) {
  $node = node_load(arg(1));
}

if (!empty($node->body[LANGUAGE_NONE][0]['value'])) {
  print $node->body[LANGUAGE_NONE][0]['value'];
  if($node->nid == 9){
    print '<img class="verified" src="/'.path_to_theme().'/images/theme/beach_yes.png" />';
  }
}

print '<div id="left-wrap">';

if (!empty($node->field_on_the_beach_verification[LANGUAGE_NONE][0]['value']) && $node->field_on_the_beach_verification[LANGUAGE_NONE][0]['value'] == 'Yes') {
  print '<div id="on-beach-wrap"><img src="/'.path_to_theme().'/images/theme/beach_yes.png" />';
  if(!empty($node->field_on_the_beach_location_deta[LANGUAGE_NONE][0]['value'])){
    print '<div>'.$node->field_on_the_beach_location_deta[LANGUAGE_NONE][0]['value'].'</div>';
  }
  print '</div>';
}

$event_view = module_invoke('views', 'block_view', 'events-block');
if (!empty($event_view['content']['#markup'])){
  print '<h3>Event Details</h3>';
  print '<div id="event-wrap">'.$event_view['content']['#markup'].'</div>';
}

if(!empty($node->field_key_features[LANGUAGE_NONE][0])){
  $i = 0;
  print '<div class="prof-box">
<h3>Features:</h3>';
    foreach ($node->field_key_features[LANGUAGE_NONE] as $item){
      print $node->field_key_features[LANGUAGE_NONE][$i]['value'].'<br />';
      $i++;
    }
    print '</div>';
}

if (!empty($node->location['lid'])) {
  print '<div class="prof-box"><h3>Location Details:</h3>';
    if(!empty($node->location['street'])){
      print $node->location['street'].'<br />';
    }
    if(!empty($node->location['city'])){
      print $node->location['city'].'<br />';
    }
    if(!empty($node->location['province_name'])){
      print $node->location['province_name'].'<br />';
    }
    if(!empty($node->location['postal_code'])){
      print $node->location['postal_code'].'<br />';
    }
    if(!empty($node->location['email'])){
      print '<a href="mailto:'.$node->location['email'].'">Email</a><br />';
    }
    if(!empty($node->location['phone'])){
      print $node->location['phone'].'<br />';
    }



    print '</div>';
}

if(!empty($node->field_website[LANGUAGE_NONE][0]['path'])) {
  print '<strong><a target="_blank" href="'.$node->field_website[LANGUAGE_NONE][0]['path'].'">Website</a></strong>';
}

$map_dl_view = module_invoke('views', 'block_view', 'map_upload-block');
if (!empty($map_dl_view['content']['#markup'])){
  print '<div id="map-dl-wrap"><h3>Beach Trails Map</h3>'.$map_dl_view['content']['#markup'].'</div>';
}

print '</div>';
$map_view = module_invoke('views', 'block_view', 'article_profile_map_2-block');
if (!empty($map_view['content']['#markup'])){
  print '<div id="map-wrap"><h3>In this Area</h3>'.$map_view['content']['#markup'].'</div>';
}
