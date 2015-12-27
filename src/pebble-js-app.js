Pebble.addEventListener('showConfiguration', function() {
  Pebble.openURL('http://midnight.design/binary-watchface/config/');
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  Pebble.sendAppMessage({
    showDate: configData.showDate,
    showBluetooth: configData.showBluetooth,
    theme: configData.theme
  }, function() {
    console.log('Configuration update successful');
  }, function() {
    console.log('Configuration update failed');
  });
});