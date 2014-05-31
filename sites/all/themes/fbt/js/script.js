var $ = jQuery.noConflict();

$(document).ready (function() {
    // Wrap main nav in classes that Drupal menu system doesn't
    $('nav#main-menu li a').each (function(){
	if(($(this).text() == 'Florida Beaches')) {
	    $(this).html("<span class=\"white-menu\">Florida</span><span class=\"yellow-menu\">Beaches</span>");
	}
	if(($(this).text() == 'Places to Stay')) {
	    $(this).html("<span class=\"white-menu\">Places to</span><span class=\"yellow-menu\">Stay</span>");
	}
	if(($(this).text() == 'Things To Do')) {
	    $(this).html("<span class=\"white-menu\">Things</span><span class=\"yellow-menu\">To Do</span>");
	}
	if(($(this).text() == 'Beach Bars & Dining')) {
	    $(this).html("<span class=\"white-menu\">Beach Bars &</span><span class=\"yellow-menu\">Dining</span>");
	}
	if(($(this).text() == 'Vacation Planning')) {
	    $(this).html("<span class=\"white-menu\">Vacation</span><span class=\"yellow-menu\">Planning</span>");
	}
      	if(($(this).text() == 'Trail Maps')) {
	    $(this).html("<span class=\"white-menu\">Trail</span><span class=\"yellow-menu\">Maps</span>");
	}
    });

  $('.front h1#page-title').html("<span>Welcome to Florida Beach Trails</span><span class=\"smallify\"> ℠ </span>")
});
