#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>


void getConfig(WiFiClient httpcli) {
    httpcli.println("HTTP/1.1 200 OK");
    httpcli.println("Content-Type: application/json");
    httpcli.println("Access-Control-Allow-Origin: *");
    httpcli.println("Connection: close");  // the connection will be closed after completion of the response
    httpcli.println("");

    //httpcli.println(serializeConfig());
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    httpcli.stop();
}

void postConfig(WiFiClient httpcli) {

}

String morphjs = (R"=====(
var configLoading=!1;function loadConfig(){configLoading=!0,$("#modalLabel")[0].innerText="Loading",$(".modal-footer").hide(),$(".modal").modal("show"),$.getJSON("/config",function(e){jQuery.isEmptyObject({configJSON:e})||($("#ConfigJSON")[0].innerText=JSON.stringify(e,null,4),null!=e.TimeZone&&""!=e.TimeZone&&(e.TimeZone.includes("ST")?($("#"+e.TimeZone)[0].checked=!0,$("#CustomTZOffset").hide()):($("#CustomTZ")[0].checked=!0,$("#CustomTZOffset")[0].value=e.TimeZone)),e.DST&&($("#DST")[0].checked=!0),e.Military&&($("#Military")[0].checked=!0),e.Metric?$("#Celsius")[0].checked=!0:e.Metric||($("#Fahrenheit")[0].checked=!0),e.WeatherAnimation&&($("#WeatherAnimation")[0].checked=!0),null!=e.ColorPalette&&""!=e.ColorPalette&&($('*[data-palette="'+e.ColorPalette+'"]')[0].checked=!0),null!=e.Brightness&&""!=e.Brightness&&($('*[data-brightness="'+e.Brightness+'"]')[0].checked=!0),null!=e.apiKey&&""!=e.apiKey&&($("#apiKey")[0].value=e.apiKey),null!=e.GeoLocation&&""!=e.GeoLocation&&($("#GeoLocation")[0].value=e.GeoLocation),null!=e.SSID&&""!=e.SSID&&($("#SSID")[0].value=e.SSID),null!=e.Password&&""!=e.Password&&($("#Password")[0].value=e.Password),null!=e.NTPServer&&""!=e.NTPServer&&($("#NTPServer")[0].value=e.NTPServer),null==e.Hostname||""==e.Hostname||($("#Hostname")[0].value=e.Hostname))}).done(function(){$(".modal").modal("hide")}).fail(function(){$(".modal-body")[0].innerHTML="<p>Load Failed!</p>"}),configLoading=!1}function updateConfigJSON(){configLoading||(configJSON={},"CustomTZ"==$('input[name="TimeZone"]:checked:enabled')[0].id?($("#CustomTZOffset").show(),configJSON.TimeZone=$('input[id="CustomTZOffset"]')[0].value):($("#CustomTZOffset").hide(),configJSON.TimeZone=$('input[name="TimeZone"]:checked:enabled')[0].id),configJSON.DST=$('input[id="DST"]')[0].checked,configJSON.Military=$('input[id="Military"]')[0].checked,"Celsius"==$('input[name="TemperatureUnit"]:checked:enabled')[0].id?configJSON.Metric=!0:"Fahrenheit"==$('input[name="TemperatureUnit"]:checked:enabled')[0].id&&(configJSON.Metric=!1),configJSON.WeatherAnimation=$('input[id="WeatherAnimation"]')[0].checked,configJSON.ColorPalette=$('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette,$('input[name="ColorPalette"]:checked:enabled')[0].id.includes("Custom")?$("#ColorInput").show():$("#ColorInput").hide(),configJSON.Brightness=$('input[name="Brightness"]:checked:enabled')[0].dataset.brightness,configJSON.apiKey=$('input[id="apiKey"]')[0].value,configJSON.GeoLocation=$('input[id="GeoLocation"]')[0].value,configJSON.SSID=$('input[id="SSID"]')[0].value,configJSON.Password=$('input[id="Password"]')[0].value,configJSON.NTPServer=$('input[id="NTPServer"]')[0].value,configJSON.Hostname=$('input[id="Hostname"]')[0].value.toLowerCase(),configJSON.DateFormat="M.D.Y",$("#ConfigJSON")[0].innerText=JSON.stringify(configJSON,null,4))}function saveConfig(){$("#modalLabel")[0].innerText="Saving",$(".modal-footer").hide(),$(".modal").modal("show");var e=$("#ConfigJSON")[0].innerText;let n={method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(JSON.parse(e))};fetch("/config",n).then(e=>{$(".modal").modal("hide")}).catch(e=>{$(".modal-body")[0].innerHTML="<p>Save Failed!</p>"})}function resetConfig(){$(".modal-footer").hide(),$("#modalLabel")[0].innerText="Reseting",$(".modal-body")[0].innerHTML='<div class="spinner-border" role="status" id="spinner"></div>',(reset={}).ConfigReset="true",resetJSON=JSON.stringify(reset,null,4);let e={method:"POST",headers:{"Content-Type":"application/json"},body:resetJSON};fetch("/reset",e).then(e=>{setTimeout(function(){},2e4),location.reload()}).catch(e=>{$(".modal-body")[0].innerHTML="<p>Reset Failed!</p>"})}function resetModal(){$("#modalLabel")[0].innerText="Reset Config",$(".modal-body")[0].innerHTML="<p>This will clear the crrent config and restore the default values from the params.h file.</p><br><p>Are you sure you want to continue?</p>",$(".modal-footer").show(),$(".modal").modal("show")}$(window).on("load",function(){loadConfig()});
)=====");

void morphJS(WiFiClient httpcli) {
    httpcli.println("HTTP/1.1 200 OK");
    httpcli.println("Content-Type: text/javascript");
    httpcli.println("Access-Control-Allow-Origin: *");
    httpcli.println("Connection: close");  // the connection will be closed after completion of the response
    httpcli.println("");
    httpcli.println("   var configLoading = false;");
    //httpcli.println("   var modal = bootstrap.Modal.getOrCreateInstance('#loading');");

    httpcli.println("	function loadConfig() {");
    httpcli.println("       configLoading = true;");
    httpcli.println("       $('#modalLabel')[0].innerText = 'Loading';");
    httpcli.println("       $('.modal-footer').hide();");
    httpcli.println("       $('.modal').modal('show');");
    httpcli.println("       $.getJSON('/config', function(configJSON) {");
    httpcli.println("           if (!jQuery.isEmptyObject({configJSON})) {");
    httpcli.println("               $(\"#ConfigJSON\")[0].innerText=JSON.stringify(configJSON, null, 4);");
    httpcli.println("                    if (configJSON.TimeZone != null && configJSON.TimeZone != \"\") {");
    httpcli.println("                        if (configJSON.TimeZone.includes('ST')) {");
    httpcli.println("                           $('#'+configJSON.TimeZone)[0].checked = true;");
    httpcli.println("                           $('#CustomTZOffset').hide();");
    httpcli.println("                        }");
    httpcli.println("                        else {");
    httpcli.println("                            $('#CustomTZ')[0].checked = true;");
    httpcli.println("                            $('#CustomTZOffset')[0].value=configJSON.TimeZone;");
    httpcli.println("                        }");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.DST) $('#DST')[0].checked = true;");
    httpcli.println("                    if (configJSON.Military) $('#Military')[0].checked = true;");
    httpcli.println("                    if (configJSON.Metric) $('#Celsius')[0].checked = true;");
    httpcli.println("                    else if (!configJSON.Metric) $('#Fahrenheit')[0].checked = true;");
    httpcli.println("                    if (configJSON.WeatherAnimation) $('#WeatherAnimation')[0].checked = true;");
    httpcli.println("                    if (configJSON.ColorPalette != null && configJSON.ColorPalette != \"\") {");
    httpcli.println("                       $('*[data-palette=\"'+configJSON.ColorPalette+'\"]')[0].checked = true;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.Brightness != null && configJSON.Brightness != \"\") {");
    httpcli.println("                        $('*[data-brightness=\"'+configJSON.Brightness+'\"]')[0].checked = true;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.apiKey != null && configJSON.apiKey != \"\") {");
    httpcli.println("                        $(\"#apiKey\")[0].value=configJSON.apiKey;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.GeoLocation != null && configJSON.GeoLocation != \"\") {");
    httpcli.println("                        $(\"#GeoLocation\")[0].value=configJSON.GeoLocation;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.SSID != null && configJSON.SSID != \"\") {");
    httpcli.println("                        $(\"#SSID\")[0].value=configJSON.SSID;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.Password != null && configJSON.Password != \"\") {");
    httpcli.println("                        $(\"#Password\")[0].value=configJSON.Password;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.NTPServer != null && configJSON.NTPServer != \"\") {");
    httpcli.println("                        $(\"#NTPServer\")[0].value=configJSON.NTPServer;");
    httpcli.println("                    }");
    httpcli.println("                    if (configJSON.Hostname != null && configJSON.Hostname != \"\") {");
    httpcli.println("                        $(\"#Hostname\")[0].value=configJSON.Hostname;");
    httpcli.println("                    }");
    httpcli.println("                }");
    httpcli.println("           })");
    httpcli.println("           .done(function() {");
    httpcli.println("               $('.modal').modal('hide');");
    httpcli.println("           })");
    httpcli.println("           .fail(function() {");
    httpcli.println("               $('.modal-body')[0].innerHTML = '<p>Load Failed!</p>';");
    httpcli.println("            });");
    httpcli.println("            configLoading = false;");
    httpcli.println("        }");

    httpcli.println("		function updateConfigJSON() {");
    httpcli.println("            if (!configLoading) {");
    httpcli.println("                configJSON = {};");
    httpcli.println("                if ($('input[name=\"TimeZone\"]:checked:enabled')[0].id == \"CustomTZ\") {");
    httpcli.println("                   $('#CustomTZOffset').show();");
    httpcli.println("                   configJSON[\"TimeZone\"] = $('input[id=\"CustomTZOffset\"]')[0].value;");
    httpcli.println("                }");
    httpcli.println("                else {");
    httpcli.println("                   $('#CustomTZOffset').hide();");
    httpcli.println("                   configJSON[\"TimeZone\"] = $('input[name=\"TimeZone\"]:checked:enabled')[0].id;");
    httpcli.println("                }");
    httpcli.println("                configJSON[\"DST\"] = $('input[id=\"DST\"]')[0].checked;");
    httpcli.println("                configJSON[\"Military\"] = $('input[id=\"Military\"]')[0].checked;");
    httpcli.println("                if ($('input[name=\"TemperatureUnit\"]:checked:enabled')[0].id == \"Celsius\") {configJSON[\"Metric\"] = true}");
    httpcli.println("                else if ($('input[name=\"TemperatureUnit\"]:checked:enabled')[0].id == \"Fahrenheit\") {configJSON[\"Metric\"] = false}");
    httpcli.println("                configJSON[\"WeatherAnimation\"] = $('input[id=\"WeatherAnimation\"]')[0].checked;");
    httpcli.println("                configJSON[\"ColorPalette\"] = $('input[name=\"ColorPalette\"]:checked:enabled')[0].dataset.palette");
    httpcli.println("                if ($('input[name=\"ColorPalette\"]:checked:enabled')[0].id.includes('Custom')) {");
    httpcli.println("                  $('#ColorInput').show();");
    httpcli.println("                }");
    httpcli.println("                else {");
    httpcli.println("                  $('#ColorInput').hide();");
    httpcli.println("                }");
    httpcli.println("                configJSON[\"Brightness\"] = $('input[name=\"Brightness\"]:checked:enabled')[0].dataset.brightness");
    httpcli.println("                configJSON[\"apiKey\"] = $('input[id=\"apiKey\"]')[0].value;");
    httpcli.println("                configJSON[\"GeoLocation\"] = $('input[id=\"GeoLocation\"]')[0].value;");
    httpcli.println("                configJSON[\"SSID\"] = $('input[id=\"SSID\"]')[0].value;");
    httpcli.println("                configJSON[\"Password\"] = $('input[id=\"Password\"]')[0].value;");
    httpcli.println("                configJSON[\"NTPServer\"] = $('input[id=\"NTPServer\"]')[0].value;");
    httpcli.println("                configJSON[\"Hostname\"] = ($('input[id=\"Hostname\"]')[0].value).toLowerCase();");
    httpcli.println("                configJSON[\"DateFormat\"] = \"M.D.Y\";");
    httpcli.println("                $(\"#ConfigJSON\")[0].innerText=JSON.stringify(configJSON, null, 4);");
    httpcli.println("            }");
    httpcli.println("        }");

    httpcli.println("        function saveConfig() {");
    httpcli.println("            $('#modalLabel')[0].innerText = 'Saving';");
    httpcli.println("            $('.modal-footer').hide();");
    httpcli.println("            $('.modal').modal('show');");
    httpcli.println("            const apiUrl = '/config';");
    httpcli.println("            var configJSON = $('#ConfigJSON')[0].innerText;");
    httpcli.println("            const requestOptions = {");
    httpcli.println("              method: 'POST',");
    httpcli.println("              headers: {");
    httpcli.println("                'Content-Type': 'application/json',");
    httpcli.println("              },");
    httpcli.println("              body: JSON.stringify(JSON.parse(configJSON)),");
    httpcli.println("            };");
    httpcli.println("            fetch(apiUrl, requestOptions)");
    httpcli.println("            .then(response => {");
    httpcli.println("              $('.modal').modal('hide');");
    httpcli.println("            })");
    httpcli.println("            .catch(error => {");
    httpcli.println("              $('.modal-body')[0].innerHTML = '<p>Save Failed!</p>';");
    httpcli.println("            ('Error:', error);");
    httpcli.println("            });");
    httpcli.println("        }");

    httpcli.println("        function resetConfig() {");
    httpcli.println("          $('.modal-footer').hide();");
    httpcli.println("          $('#modalLabel')[0].innerText = 'Reseting';");
    httpcli.println("          $('.modal-body')[0].innerHTML = '<div class=\"spinner-border\" role=\"status\" id=\"spinner\"></div>';");
    httpcli.println("          reset = {};");
    httpcli.println("          reset[\"ConfigReset\"] = \"true\";");
    httpcli.println("          resetJSON = JSON.stringify(reset, null, 4);");
    httpcli.println("          const apiUrl = '/reset';");
    httpcli.println("          const requestOptions = {");
    httpcli.println("            method: 'POST',");
    httpcli.println("            headers: {");
    httpcli.println("              'Content-Type': 'application/json',");
    httpcli.println("            },");
    httpcli.println("            body: resetJSON,");
    httpcli.println("          };");
    httpcli.println("          fetch(apiUrl, requestOptions)");
    httpcli.println("          .then(response => {");
    httpcli.println("            setTimeout(function () { }, 20000);");
    httpcli.println("            location.reload();");
    httpcli.println("          })");
    httpcli.println("          .catch(error => {");
    httpcli.println("            $('.modal-body')[0].innerHTML = '<p>Reset Failed!</p>';");
    httpcli.println("          ('Error:', error);");
    httpcli.println("          });");
    httpcli.println("        }");

    httpcli.println("        function resetModal() {");
    httpcli.println("            $('#modalLabel')[0].innerText = 'Reset Config';");
    httpcli.println("            $('.modal-body')[0].innerHTML = '<p>This will clear the crrent config and restore the default values from the params.h file.</p><br><p>Are you sure you want to continue?</p>';");
    httpcli.println("            $('.modal-footer').show();");
    httpcli.println("            $('.modal').modal('show');");
    httpcli.println("        }");
        
    httpcli.println("       $(window).on('load', function(){");
    httpcli.println("           loadConfig();");
    httpcli.println("       }); ");

    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    httpcli.stop();
    return;
}

void morphJS2(WiFiClient httpcli) {
    httpcli.println("HTTP/1.1 200 OK");
    httpcli.println("Content-Type: text/javascript");
    httpcli.println("Access-Control-Allow-Origin: *");
    httpcli.println("Connection: close");  // the connection will be closed after completion of the response
    httpcli.println("");
    httpcli.println(morphjs);
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    httpcli.stop();
    return;
}

String indexHTML = (R"=====(
<!doctypehtml><html lang=en><meta charset=utf-8><meta content="width=device-width,initial-scale=1"name=viewport><link crossorigin=anonymous href=https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css integrity=sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3 rel=stylesheet><title>MORPH CLOCK CONFIG</title><div class=container-fluid><header class="d-flex border-bottom flex-wrap justify-content-center mb-4 py-3"><a href=/ class="d-flex align-items-center mb-3 mb-md-0 me-md-auto text-dark text-decoration-none"><svg class="bi bi-clock-fill"fill=currentColor height=32 viewBox="0 0 16 16"width=32 xmlns=http://www.w3.org/2000/svg><path d="M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0M8 3.5a.5.5 0 0 0-1 0V9a.5.5 0 0 0 .252.434l3.5 2a.5.5 0 0 0 .496-.868L8 8.71z"/></svg> <span class=fs-4>MORPH CLOCK CONFIG</span></a></header><div class=row><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>Time Zone</h5><div class=form-check><input class=form-check-input id=EST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=EST>Eastern Standard Time</label></div><div class=form-check><input class=form-check-input id=CST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=CST>Central Standard Time</label></div><div class=form-check><input class=form-check-input id=MST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=MST>Mountain Standard Time</label></div><div class=form-check><input class=form-check-input id=PST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=PST>Pacific Standard Time</label></div><div class=form-check><input class=form-check-input id=AST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=AST>Alaska Standard Time</label></div><div class=form-check><input class=form-check-input id=HAST onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=HAST>Hawaii-Aleutian Standard Time</label></div><div class=form-check><input class=form-check-input id=CustomTZ onchange=updateConfigJSON() type=radio name=TimeZone> <label class=form-check-label for=CustomTZ>Custom (UTC offset eg, -5)</label> <input class=form-control id=CustomTZOffset onchange=updateConfigJSON() placeholder="UTC Offset"></div></div></div><div class=card><div class=card-body><h5 class=card-title>Daylight Savings</h5><div class="form-check form-switch"><input class=form-check-input id=DST onchange=updateConfigJSON() type=checkbox> <label class=form-check-label for=DST></label></div></div></div><div class=card><div class=card-body><h5 class=card-title>Military Time</h5><div class="form-check form-switch"><input class=form-check-input id=Military onchange=updateConfigJSON() type=checkbox> <label class=form-check-label for=Military></label></div></div></div></div><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>Temperature Unit</h5><div class=form-check><input class=form-check-input id=Fahrenheit onchange=updateConfigJSON() type=radio name=TemperatureUnit data-value=N> <label class=form-check-label for=Fahrenheit>Fahrenheit</label></div><div class=form-check><input class=form-check-input id=Celsius onchange=updateConfigJSON() type=radio name=TemperatureUnit data-value=Y> <label class=form-check-label for=Celsius>Celsius</label></div></div></div><div class=card><div class=card-body><h5 class=card-title>Weather Animation</h5><div class="form-check form-switch"><input class=form-check-input id=WeatherAnimation onchange=updateConfigJSON() type=checkbox> <label class=form-check-label for=WeatherAnimation></label></div></div></div></div><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>Color Palette</h5><div class=row><div class="col col-6"><div class=form-check><input class=form-check-input id=Cyan onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=1> <label class=form-check-label for=Cyan>Cyan</label></div><div class=form-check><input class=form-check-input id=Red onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=2> <label class=form-check-label for=Red>Red</label></div><div class=form-check><input class=form-check-input id=Blue onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=3> <label class=form-check-label for=Blue>Blue</label></div><div class=form-check><input class=form-check-input id=Yellow onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=4> <label class=form-check-label for=Yellow>Yellow</label></div><div class=form-check><input class=form-check-input id=BrightBlue onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=5> <label class=form-check-label for=BrightBlue>Bright Blue</label></div><div class=form-check><input class=form-check-input id=Orange onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=6> <label class=form-check-label for=Orange>Orange</label></div><div class=form-check><input class=form-check-input id=Green onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=7> <label class=form-check-label for=Green>Green</label></div><div class=form-check><input class=form-check-input id=Custom1 onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=8> <label class=form-check-label for=Custom1>Custom 1</label></div><div class=form-check><input class=form-check-input id=Custom2 onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=9> <label class=form-check-label for=Custom2>Custom 2</label></div><div class=form-check><input class=form-check-input id=Custom3 onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=10> <label class=form-check-label for=Custom3>Custom 3</label></div><div class=form-check><input class=form-check-input id=Custom4 onchange=updateConfigJSON() type=radio name=ColorPalette data-palette=11> <label class=form-check-label for=Custom4>Custom 4</label></div></div><div class="col col-6"><div id=ColorInput><label class=form-label for=TimeColorInput>Time</label> <input class="form-control form-control-color"id=TimeColorInput type=color title="Choose time color"value=#563d7c> <label class=form-label for=WindColorInput>Wind</label> <input class="form-control form-control-color"id=WindColorInput type=color title="Choose wind color"value=#563d7c> <label class=form-label for=DateColorInput>Date</label> <input class="form-control form-control-color"id=DateColorInput type=color title="Choose date color"value=#563d7c> <label class=form-label for=WeatherColorInput>Weather</label> <input class="form-control form-control-color"id=WeatherColorInput type=color title="Choose weather color"value=#563d7c></div></div></div></div></div><div class=card><div class=card-body><h5 class=card-title>Brightness</h5><div class=form-check><input class=form-check-input id=BrightnessAuto onchange=updateConfigJSON() type=radio name=Brightness data-brightness=Auto> <label class=form-check-label for=BrightnessAuto>Auto</label></div><div class=form-check><input class=form-check-input id=Brightness70 onchange=updateConfigJSON() type=radio name=Brightness data-brightness=70> <label class=form-check-label for=Brightness70>70</label></div><div class=form-check><input class=form-check-input id=Brightness35 onchange=updateConfigJSON() type=radio name=Brightness data-brightness=35> <label class=form-check-label for=Brightness35>35</label></div><div class=form-check><input class=form-check-input id=BrightnessOff onchange=updateConfigJSON() type=radio name=Brightness data-brightness=0> <label class=form-check-label for=BrightnessOff>Off</label></div></div></div></div></div><div class=row><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>Open Weather Map</h5><p class=card-text><a href=https://www.openweathermap.org target=_blank>openweathermap.org</a></p><label class=form-label for=apiKey>API key</label> <input class=form-control id=apiKey onchange=updateConfigJSON() placeholder="API key"> <label class=form-label for=GeoLocation>Geo Location: City,Country</label> <input class=form-control id=GeoLocation onchange=updateConfigJSON() placeholder=City,Country></div></div></div><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>WiFi</h5><label class=form-label for=SSID>SSID</label> <input class=form-control id=SSID onchange=updateConfigJSON() placeholder=SSID> <label class=form-label for=Password>Password</label> <input class=form-control id=Password onchange=updateConfigJSON() placeholder=Password></div></div></div><div class="col col-4"><div class=card><div class=card-body><h5 class=card-title>Hostname</h5><input class=form-control id=Hostname onchange=updateConfigJSON() placeholder=Hostname></div></div><div class=card><div class=card-body><h5 class=card-title>NTP Server</h5><input class=form-control id=NTPServer onchange=updateConfigJSON() placeholder="NTP Server"></div></div></div></div><div class=row><div class="col col-12"><button class="btn btn-primary"type=button onclick=saveConfig()>Save</button> <button class="btn btn-danger"type=button onclick=resetModal()>Reset Config</button></div></div><div class=row><div class="col col-12"><div class="accordion accordion-flush"id=accordionConfigJSON><div class=accordion-item><h2 class=accordion-header id=flush-headingOne><button class="accordion-button collapsed"type=button aria-controls=flush-collapseOne aria-expanded=false data-bs-target=#flush-collapseOne data-bs-toggle=collapse>Config JSON</button></h2><div class="accordion-collapse collapse"id=flush-collapseOne aria-labelledby=flush-headingOne data-bs-parent=#accordionConfigJSON><div class=accordion-body><div id=ConfigJSON></div></div></div></div></div></div></div><div class=modal id=loading aria-labelledby=exampleModalLabel aria-hidden=true tabindex=-1><div class="modal-dialog modal-dialog-centered"><div class=modal-content><div class=modal-header><h5 class=modal-title id=modalLabel></h5><button class=btn-close type=button data-bs-dismiss=modal aria-label=Close></button></div><div class=modal-body><div class=spinner-border id=spinner role=status></div></div><div class=modal-footer><button class="btn btn-primary"type=button data-bs-dismiss=modal>No</button> <button class="btn btn-danger"type=button onclick=resetConfig()>Yes</button></div></div></div></div></div><script defer src=https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.min.js crossorigin=anonymous integrity="sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo="></script><script defer src=https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js crossorigin=anonymous integrity=sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p></script><script defer src=/morph.js></script>
)=====");

void index(WiFiClient httpcli) {
    httpcli.println("HTTP/1.1 200 OK");
    httpcli.println("Content-type: text/html");
    httpcli.println("Access-Control-Allow-Origin: *");
    httpcli.println("Connection: close");  // the connection will be closed after completion of the response
    httpcli.println("");
    httpcli.println("<!DOCTYPE HTML>");
    httpcli.println("<html>");

    //head
    httpcli.println("<head>");
    httpcli.println(" <meta charset=\"utf-8\">");
    httpcli.println(" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    httpcli.println(" <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3\" crossorigin=\"anonymous\">");
    httpcli.println(" <title>MORPH CLOCK CONFIG</title>");
    httpcli.println("</head>");

    //body
    httpcli.println("<body>");
    httpcli.println("<div class=\"container-fluid\">");
    httpcli.println("<header class=\"d-flex flex-wrap justify-content-center py-3 mb-4 border-bottom\">");
    httpcli.println("<a href=\"/\" class=\"d-flex align-items-center mb-3 mb-md-0 me-md-auto text-dark text-decoration-none\">");
    httpcli.println("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32\" height=\"32\" fill=\"currentColor\" class=\"bi bi-clock-fill\" viewBox=\"0 0 16 16\">");
    httpcli.println("<path d=\"M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0M8 3.5a.5.5 0 0 0-1 0V9a.5.5 0 0 0 .252.434l3.5 2a.5.5 0 0 0 .496-.868L8 8.71z\"/>");
    httpcli.println("</svg>");
    httpcli.println("<span class=\"fs-4\">MORPH CLOCK CONFIG</span>");
    httpcli.println("</a>");
    httpcli.println("</header>");

    //Row 1
    httpcli.println("<div class=\"row\">");
    httpcli.println("<div class=\"col col-4\">");
        httpcli.println("<div class=\"card\">");
        httpcli.println("<div class=\"card-body\">");
        httpcli.println("<h5 class=\"card-title\">Time Zone</h5>");
        httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"EST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"EST\">");
            httpcli.println("Eastern Standard Time");
            httpcli.println("</label>");
        httpcli.println("</div>");
        httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"CST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"CST\">");
            httpcli.println("Central Standard Time");
            httpcli.println("</label>");
            httpcli.println("</div>");
            httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"MST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"MST\">");
                httpcli.println("Mountain Standard Time");
            httpcli.println("</label>");
            httpcli.println("</div>");
            httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"PST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"PST\">");
                httpcli.println("Pacific Standard Time");
            httpcli.println("</label>");
            httpcli.println("</div>");
            httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"AST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"AST\">");
                httpcli.println("Alaska Standard Time");
            httpcli.println("</label>");
            httpcli.println("</div>");
            httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"HAST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"HAST\">");
                httpcli.println("Hawaii-Aleutian Standard Time");
            httpcli.println("</label>");
            httpcli.println("</div>");
            httpcli.println("<div class=\"form-check\">");
            httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TimeZone\" id=\"CustomTZ\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"CustomTZ\">");
                httpcli.println("Custom (UTC offset eg, -5)");
            httpcli.println("</label>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"CustomTZOffset\" placeholder=\"UTC Offset\" onchange=\"updateConfigJSON()\">");
            httpcli.println("</div>");
        httpcli.println("</div>");
        httpcli.println("</div>");
        httpcli.println("<div class=\"card\">");
        httpcli.println("<div class=\"card-body\">");
            httpcli.println("<h5 class=\"card-title\">Daylight Savings</h5>");
            httpcli.println("<div class=\"form-check form-switch\">");
            httpcli.println("<input class=\"form-check-input\" type=\"checkbox\" id=\"DST\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label class=\"form-check-label\" for=\"DST\"></label>");
            httpcli.println("</div>");
        httpcli.println("</div>");
        httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"col col-4\">");
    httpcli.println("<div class=\"card\">");
    httpcli.println("<div class=\"card-body\">");
    httpcli.println("<h5 class=\"card-title\">Military Time</h5>");
    httpcli.println("<div class=\"form-check form-switch\">");
    httpcli.println("<input class=\"form-check-input\" type=\"checkbox\" id=\"Military\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Military\"></label>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"card\">");
    httpcli.println("<div class=\"card-body\">");
    httpcli.println("<h5 class=\"card-title\">Temperature Unit</h5>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TemperatureUnit\" id=\"Fahrenheit\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Fahrenheit\">");
    httpcli.println("Fahrenheit");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"TemperatureUnit\" id=\"Celsius\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Celsius\">");
    httpcli.println("Celsius");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"card\">");
    httpcli.println("<div class=\"card-body\">");
    httpcli.println("<h5 class=\"card-title\">Weather Animation</h5>");
    httpcli.println("<div class=\"form-check form-switch\">");
    httpcli.println("<input class=\"form-check-input\" type=\"checkbox\" id=\"WeatherAnimation\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"WeatherAnimation\"></label>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"col col-4\">");
    httpcli.println("<div class=\"card\">");
    httpcli.println("<div class=\"card-body\">");
    httpcli.println("<h5 class=\"card-title\">Color Palette</h5>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Cyan\" data-palette=\"1\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Cyan\">");
    httpcli.println("Cyan");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Red\" data-palette=\"2\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Red\">");
    httpcli.println("Red");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Blue\" data-palette=\"3\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Blue\">");
    httpcli.println("Blue");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Yellow\" data-palette=\"4\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Yellow\">");
    httpcli.println("Yellow");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"BrightBlue\" data-palette=\"5\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"BrightBlue\">");
    httpcli.println("Bright Blue");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Orange\" data-palette=\"6\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Orange\">");
    httpcli.println("Orange");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Green\" data-palette=\"7\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Green\">");
    httpcli.println("Green");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Custom1\" data-palette=\"8\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Custom1\">");
    httpcli.println("Custom 1");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Custom2\" data-palette=\"9\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Custom2\">");
    httpcli.println("Custom 2");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Custom3\" data-palette=\"10\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Custom3\">");
    httpcli.println("Custom 3");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"ColorPalette\" id=\"Custom4\" data-palette=\"11\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Custom4\">");
    httpcli.println("Custom 4");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"card\">");
    httpcli.println("<div class=\"card-body\">");
    httpcli.println("<h5 class=\"card-title\">Brightness</h5>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"Brightness\" id=\"BrightnessAuto\" data-brightness=\"Auto\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"BrightnessAuto\">");
    httpcli.println("Auto");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"Brightness\" id=\"Brightness70\" data-brightness=\"70\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Brightness70\">");
    httpcli.println("70");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"Brightness\" id=\"Brightness35\" data-brightness=\"35\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"Brightness35\">");
    httpcli.println("35");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"form-check\">");
    httpcli.println("<input class=\"form-check-input\" type=\"radio\" name=\"Brightness\" id=\"BrightnessOff\" data-brightness=\"0\" onchange=\"updateConfigJSON()\">");
    httpcli.println("<label class=\"form-check-label\" for=\"BrightnessOff\">");
    httpcli.println("Off");
    httpcli.println("</label>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");

    //Row 2
    httpcli.println("<div class=\"row\">");
    httpcli.println("<div class=\"col col-4\">");
        httpcli.println("<div class=\"card\">");
        httpcli.println("<div class=\"card-body\">");
            httpcli.println("<h5 class=\"card-title\">Open Weather Map</h5>");
            httpcli.println("<p class=\"card-text\"><a href=\"https://www.openweathermap.org\" target=\"_blank\">openweathermap.org</a></p>");
            httpcli.println("<label for=\"apiKey\" class=\"form-label\">API key</label>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"apiKey\" placeholder=\"API key\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label for=\"GeoLocation\" class=\"form-label\">Geo Location: City,Country</label>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"GeoLocation\" placeholder=\"City,Country\" onchange=\"updateConfigJSON()\">");
        httpcli.println("</div>");
        httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"col col-4\">");
        httpcli.println("<div class=\"card\">");
        httpcli.println("<div class=\"card-body\">");
            httpcli.println("<h5 class=\"card-title\">WiFi</h5>");
            httpcli.println("<label for=\"SSID\" class=\"form-label\">SSID</label>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"SSID\" placeholder=\"SSID\" onchange=\"updateConfigJSON()\">");
            httpcli.println("<label for=\"Password\" class=\"form-label\">Password</label>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"Password\" placeholder=\"Password\" onchange=\"updateConfigJSON()\">");
        httpcli.println("</div>");
        httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("<div class=\"col col-4\">");
        httpcli.println("<div class=\"card\">");
                httpcli.println("<div class=\"card-body\">");
                    httpcli.println("<h5 class=\"card-title\">Hostname</h5>");
                    httpcli.println("<input type=\"text\" class=\"form-control\" id=\"Hostname\" placeholder=\"Hostname\" onchange=\"updateConfigJSON()\">");
                httpcli.println("</div>");
            httpcli.println("</div>");
        httpcli.println("<div class=\"card\">");
        httpcli.println("<div class=\"card-body\">");
            httpcli.println("<h5 class=\"card-title\">NTP Server</h5>");
            httpcli.println("<input type=\"text\" class=\"form-control\" id=\"NTPServer\" placeholder=\"NTP Server\" onchange=\"updateConfigJSON()\">");
        httpcli.println("</div>");
        httpcli.println("</div>");
    httpcli.println("</div>");
    httpcli.println("</div>");

    //Row 3
    httpcli.println("<div class=\"row\">");
        httpcli.println("<div class=\"col col-12\">");
            httpcli.println("<button type=\"button\" class=\"btn btn-primary\" onclick=\"saveConfig()\">Save</button>");
            httpcli.println("<button type=\"button\" class=\"btn btn-danger\" onclick=\"resetModal()\">Reset Config</button>");
        httpcli.println("</div>");
    httpcli.println("</div>");

    //Row4
    httpcli.println("<div class=\"row\">");
        httpcli.println("<div class=\"col col-12\">");
            httpcli.println("<div class=\"accordion accordion-flush\" id=\"accordionConfigJSON\">");
            httpcli.println("<div class=\"accordion-item\">");
                httpcli.println("<h2 class=\"accordion-header\" id=\"flush-headingOne\">");
                httpcli.println("<button class=\"accordion-button collapsed\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#flush-collapseOne\" aria-expanded=\"false\" aria-controls=\"flush-collapseOne\">");
                    httpcli.println("Config JSON");
                httpcli.println("</button>");
                httpcli.println("</h2>");
                httpcli.println("<div id=\"flush-collapseOne\" class=\"accordion-collapse collapse\" aria-labelledby=\"flush-headingOne\" data-bs-parent=\"#accordionConfigJSON\">");
                httpcli.println("<div class=\"accordion-body\">");
                    httpcli.println("<div id=\"ConfigJSON\"></div>");
                httpcli.println("</div>");
                httpcli.println("</div>");
            httpcli.println("</div>");
            httpcli.println("</div>");
        httpcli.println("</div>");
    httpcli.println("</div>");

    //Modal
    httpcli.println("<div class=\"modal\" id=\"loading\" tabindex=\"-1\" aria-labelledby=\"exampleModalLabel\" aria-hidden=\"true\">");
    httpcli.println("      <div class=\"modal-dialog modal-dialog-centered\">");
    httpcli.println("        <div class=\"modal-content\">");
    httpcli.println("          <div class=\"modal-header\">");
    httpcli.println("            <h5 class=\"modal-title\" id=\"modalLabel\"></h5>");
    httpcli.println("            <button type=\"button\" class=\"btn-close\" data-bs-dismiss=\"modal\" aria-label=\"Close\"></button>");
    httpcli.println("          </div>");
    httpcli.println("          <div class=\"modal-body\">");
    httpcli.println("              <div class=\"spinner-border\" role=\"status\" id=\"spinner\"></div>");
    httpcli.println("          </div>");
    httpcli.println("           <div class=\"modal-footer\">");
    httpcli.println("               <button type=\"button\" class=\"btn btn-primary\" data-bs-dismiss=\"modal\">No</button>");
    httpcli.println("               <button type=\"button\" class=\"btn btn-danger\" onclick=\"resetConfig()\">Yes</button>");
    httpcli.println("           </div>");
    httpcli.println("        </div>");
    httpcli.println("      </div>");
    httpcli.println("    </div>");
    httpcli.println("  </div>");

    //Scripts
    httpcli.println("<script defer src=\"https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.min.js\" integrity=\"sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo=\" crossorigin=\"anonymous\"></script>");
    httpcli.println("<script defer src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p\" crossorigin=\"anonymous\"></script>");
    httpcli.println("<script defer src=\"/morph.js\" type=\"text/javascript\"></script>");
    httpcli.println("</div>");
    httpcli.println("</body>");

    httpcli.println("</html>");
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    httpcli.stop();
}

void index2(WiFiClient httpcli) {
    httpcli.println("HTTP/1.1 200 OK");
    httpcli.println("Content-type: text/html");
    httpcli.println("Access-Control-Allow-Origin: *");
    httpcli.println("Connection: close");  // the connection will be closed after completion of the response
    httpcli.println("");
    httpcli.println(String(indexHTML));
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    httpcli.stop();
}