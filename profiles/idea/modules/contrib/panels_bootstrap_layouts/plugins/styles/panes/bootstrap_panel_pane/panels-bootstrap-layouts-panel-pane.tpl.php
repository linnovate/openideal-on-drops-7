<div <?php if ($content->css_id) print "id='$content->css_id'" ?> class="panel <?php print implode(' ', array($classes, $content->css_class))?>">
  <?php if ($content->title): ?>
    <div class="panel-heading">
      <h3 class="panel-title">
        <?php print $content->title ?>
      </h3>
    </div>
  <?php endif ?>

  <?php if ($add_padding): ?>
    <div class="panel-body">
  <?php endif ?>

  <?php print render($content->content) ?>

  <?php if ($add_padding): ?>
    </div>
  <?php endif ?>
</div>
