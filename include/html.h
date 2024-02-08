#pragma once

#include <Arduino.h>  // PROGMEM

const char index_html[] PROGMEM = R"***(<!doctypehtml><html lang="en"><meta charset="utf-8"><meta name="viewport"content="width=device-width,initial-scale=1"><link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css"rel="stylesheet"integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3"crossorigin="anonymous"><title>MORPH CLOCK CONFIG</title><div class="container"><header class="d-flex flex-wrap justify-content-center py-3 mb-4 border-bottom"><a href="/"class="d-flex align-items-center mb-3 mb-md-0 me-md-auto text-dark text-decoration-none"><svg xmlns="http://www.w3.org/2000/svg"width="32"height="32"fill="currentColor"class="bi bi-clock-fill"viewBox="0 0 16 16"><path d="M16 8A8 8 0 1 1 0 8a8 8 0 0 1 16 0M8 3.5a.5.5 0 0 0-1 0V9a.5.5 0 0 0 .252.434l3.5 2a.5.5 0 0 0 .496-.868L8 8.71z"/></svg> <span class="fs-4">MORPH CLOCK CONFIG</span></a></header><div class="row"><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">Time Zone</h5><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="EST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="EST">Eastern Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="CST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="CST">Central Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="MST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="MST">Mountain Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="PST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="PST">Pacific Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="AST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="AST">Alaska Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="HAST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="HAST">Hawaii-Aleutian Standard Time</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TimeZone"id="CustomTZ"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="CustomTZ">Custom (UTC offset eg, -5)</label> <input class="form-control"id="CustomTZOffset"placeholder="UTC Offset"onchange="updateConfigJSON(event)"></div></div></div><div class="card"><div class="card-body"><h5 class="card-title">Daylight Savings</h5><div class="form-check form-switch"><input class="form-check-input"type="checkbox"id="DST"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="DST"></label></div></div></div><div class="card"><div class="card-body"><h5 class="card-title">Military Time</h5><div class="form-check form-switch"><input class="form-check-input"type="checkbox"id="Military"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Military"></label></div></div></div></div><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">Temperature Unit</h5><div class="form-check"><input class="form-check-input"type="radio"name="TemperatureUnit"id="Fahrenheit"data-value="false"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Fahrenheit">Fahrenheit</label></div><div class="form-check"><input class="form-check-input"type="radio"name="TemperatureUnit"id="Celsius"data-value="true"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Celsius">Celsius</label></div></div></div><div class="card"><div class="card-body"><h5 class="card-title">Weather Animation</h5><div class="form-check form-switch"><input class="form-check-input"type="checkbox"id="WeatherAnimation"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="WeatherAnimation"></label></div></div></div><div class="card"><div class="card-body"><h5 class="card-title">Morphing</h5><div class="form-check form-switch"><input class="form-check-input"type="checkbox"id="Morphing"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Morphing"></label></div></div></div></div><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">Color Palette</h5><div class="row"><div class="col col-6"><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Cyan"data-palette="0"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Cyan">Cyan</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Red"data-palette="1"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Red">Red</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Blue"data-palette="2"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Blue">Blue</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Yellow"data-palette="3"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Yellow">Yellow</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="BrightBlue"data-palette="4"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="BrightBlue">Bright Blue</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Orange"data-palette="5"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Orange">Orange</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Green"data-palette="6"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Green">Green</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Custom1"data-palette="7"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Custom1">Custom 1</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Custom2"data-palette="8"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Custom2">Custom 2</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Custom3"data-palette="9"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Custom3">Custom 3</label></div><div class="form-check"><input class="form-check-input"type="radio"name="ColorPalette"id="Custom4"data-palette="10"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="Custom4">Custom 4</label></div></div><div class="col col-6"><div id="ColorInput"><label for="WindColorInput"class="form-label">Wind</label> <input type="color"class="form-control form-control-color"id="WindColorInput"value="#563d7c"title="Choose wind color"onchange="updateConfigJSON(event)"> <label for="WeatherColorInput"class="form-label">Weather</label> <input type="color"class="form-control form-control-color"id="WeatherColorInput"value="#563d7c"title="Choose weather color"onchange="updateConfigJSON(event)"> <label for="TimeColorInput"class="form-label">Time</label> <input type="color"class="form-control form-control-color"id="TimeColorInput"value="#563d7c"title="Choose time color"onchange="updateConfigJSON(event)"> <label for="DateColorInput"class="form-label">Date</label> <input type="color"class="form-control form-control-color"id="DateColorInput"value="#563d7c"title="Choose date color"onchange="updateConfigJSON(event)"></div></div></div></div></div><div class="card"><div class="card-body"><h5 class="card-title">Brightness</h5><div class="form-check"><input class="form-check-input"type="radio"name="Brightness"id="BrightnessAuto"data-brightness="Auto"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="BrightnessAuto">Auto</label></div><div class="form-check"><input class="form-check-input"type="radio"name="Brightness"id="BrightnessManual"data-brightness="Manual"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="BrightnessManual">Manual</label></div><input type="range"class="form-range"id="Brightness"step="5"data-bs-toggle="tooltip"data-bs-placement="top"title="Tooltip on top"onchange="updateConfigJSON(event)"></div></div></div></div><div class="row"><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">Open Weather Map</h5><p class="card-text"><a href="https://www.openweathermap.org"target="_blank">openweathermap.org</a><br><a href="https://openweathermap.org/current"target="_blank">OpenWeatherMap API</a></p><label for="apiKey"class="form-label">API key</label> <input class="form-control"id="apiKey"placeholder="API key"onchange="updateConfigJSON(event)"> <label for="apiKey"class="form-label">Location:</label><div class="form-check"><input class="form-check-input"type="radio"name="Location"id="GeoAuto"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="GeoAuto">Auto</label></div><div class="form-check"><input class="form-check-input"type="radio"name="Location"id="GeoCoordinates"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="GeoCoordinates">Geo Coordinates</label> <input class="form-control"id="GeoLocLatText"placeholder="Latitude"onchange="updateConfigJSON(event)"> <input class="form-control"id="GeoLocLonText"placeholder="Longitude"onchange="updateConfigJSON(event)"></div><div class="form-check"><input class="form-check-input"type="radio"name="Location"id="GeoCityStateCountry"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="GeoCityStateCountry">City,State,Country</label> <input class="form-control"id="GeoCityStateCountryText"placeholder="City,State,Country"onchange="updateConfigJSON(event)"></div><div class="form-check"><input class="form-check-input"type="radio"name="Location"id="GeoCityID"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="GeoCityID">City ID</label> <input class="form-control"id="GeoCityIDText"placeholder="City ID"onchange="updateConfigJSON(event)"></div><div class="form-check"><input class="form-check-input"type="radio"name="Location"id="GeoZIPCode"onchange="updateConfigJSON(event)"> <label class="form-check-label"for="GeoZIPCode">ZIP Code</label> <input class="form-control"id="GeoZIPCodeText"placeholder="Zip,US"onchange="updateConfigJSON(event)"></div></div></div></div><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">WiFi</h5><label for="SSID"class="form-label">SSID</label> <input class="form-control"id="SSID"placeholder="SSID"onchange="updateConfigJSON(event)"> <label for="Password"class="form-label">Password</label> <input class="form-control"id="Password"placeholder="Password"onchange="updateConfigJSON(event)"></div></div></div><div class="col-md-4"><div class="card"><div class="card-body"><h5 class="card-title">Hostname</h5><input class="form-control"id="Hostname"placeholder="Hostname"onchange="updateConfigJSON(event)"></div></div><div class="card"><div class="card-body"><h5 class="card-title">NTP Server</h5><input class="form-control"id="NTPServer"placeholder="NTP Server"onchange="updateConfigJSON(event)"></div></div></div></div><div class="row"><div class="col-md-12"><button type="button"class="btn btn-primary mt-1 mr-1"onclick="saveConfig()">Save</button> <button type="button"class="btn btn-info mt-1 mr-1"onclick='$("#collapseConfig").collapse("toggle"),$("#collapseConfig").on("shown.bs.collapse",function(){this.scrollIntoView()})'>Show Config</button> <button type="button"class="btn btn-warning mt-1 mr-1"onclick="rebootModal()">Reboot</button> <button type="button"class="btn btn-danger mt-1"onclick="resetModal()">Reset Config</button></div></div><div class="row"><div class="col-md-12"><div class="collapse"id="collapseConfig"><div class="card card-body"><div id="ConfigJSON"></div></div></div></div></div><div class="modal"id="loading"tabindex="-1"aria-labelledby="ModalLabel"aria-hidden="true"><div class="modal-dialog modal-dialog-centered"><div class="modal-content"><div class="modal-header"><h5 class="modal-title"id="modalLabel"></h5><button type="button"class="btn-close"data-bs-dismiss="modal"aria-label="Close"></button></div><div class="modal-body"><div class="d-flex justify-content-center"><div class="spinner-border"role="status"><span class="visually-hidden">Loading...</span></div></div></div><div class="modal-footer"><button type="button"class="btn btn-primary"data-bs-dismiss="modal">No</button> <button id="btnReset"type="button"class="btn btn-danger"onclick="resetConfig()">Yes</button> <button id="btnReboot"type="button"class="btn btn-danger"onclick="rebootClock()">Yes</button></div></div></div></div></div><script defer="defer"src="https://cdn.jsdelivr.net/npm/jquery@3.7.1/dist/jquery.min.js"integrity="sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo="crossorigin="anonymous"></script><script defer="defer"src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js"integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p"crossorigin="anonymous"></script><script defer="defer"src="morph.js"></script>)***";

const char script_js[] PROGMEM = R"***(var configLoading=!1,cc_cyan="#001E1E",cc_ylw="#2D2D00",cc_grn="#002D00",cc_wht="#191919";function componentToHex(e){e=e.toString(16);return 1==e.length?"0"+e:e}function rgbToHex(e,o,t){return"#"+componentToHex(e)+componentToHex(o)+componentToHex(t)}function hexToRgb(e){e=/^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(e);return e?{r:parseInt(e[1],16),g:parseInt(e[2],16),b:parseInt(e[3],16)}:null}function hideGeoLocElems(){$('input[name="Location"]:checked:enabled').parent().parent().find("input[class=form-control][id^=Geo]").hide(),$("#BrowserLocation").hide()}function loadConfig(){configLoading=!0,$("#modalLabel")[0].innerText="Loading",$(".modal-footer").hide(),$(".modal").modal("show"),$.getJSON("/config",function(e){jQuery.isEmptyObject({configJSON:e})||(e.TimeZone&&(e.TimeZone.includes("ST")?($("#"+e.TimeZone)[0].checked=!0,$("#CustomTZOffset").hide()):($("#CustomTZ")[0].checked=!0,$("#CustomTZOffset")[0].value=e.TimeZone)),e.DST&&($("#DST")[0].checked=!0),e.Military&&($("#Military")[0].checked=!0),e.Metric?$("#Celsius")[0].checked=!0:e.Metric||($("#Fahrenheit")[0].checked=!0),e.WeatherAnimation&&($("#WeatherAnimation")[0].checked=!0),e.Morphing&&($("#Morphing")[0].checked=!0),e.ColorPalette&&($('*[data-palette="'+e.ColorPalette+'"]')[0].checked=!0,$('input[name="ColorPalette"]:checked:enabled')[0].id.includes("Custom")?($("#ColorInput").show(),$('input[id="TimeColorInput"]')[0].value=rgbToHex(e["ColorPalette"+e.ColorPalette].time.r,e["ColorPalette"+e.ColorPalette].time.g,e["ColorPalette"+e.ColorPalette].time.b),$('input[id="WindColorInput"]')[0].value=rgbToHex(e["ColorPalette"+e.ColorPalette].wind.r,e["ColorPalette"+e.ColorPalette].wind.g,e["ColorPalette"+e.ColorPalette].wind.b),$('input[id="DateColorInput"]')[0].value=rgbToHex(e["ColorPalette"+e.ColorPalette].date.r,e["ColorPalette"+e.ColorPalette].date.g,e["ColorPalette"+e.ColorPalette].date.b),$('input[id="WeatherColorInput"]')[0].value=rgbToHex(e["ColorPalette"+e.ColorPalette].weather.r,e["ColorPalette"+e.ColorPalette].weather.g,e["ColorPalette"+e.ColorPalette].weather.b)):$("#ColorInput").hide()),e.Brightness&&("Auto"==e.Brightness?($("#BrightnessAuto")[0].checked=!0,$("#Brightness").hide()):($("#BrightnessManual")[0].checked=!0,$("#Brightness").show(),e.BrightnessValue?e.BrightnessValue?$("#Brightness")[0].value=e.BrightnessValue=e.Brightness:$("#Brightness")[0].value=42:e.BrightnessValue=$("#Brightness")[0].value=e.Brightness,$('label[for="BrightnessManual"]')[0].innerText="Manual: "+$("#Brightness")[0].value)),e.apiKey&&($("#apiKey")[0].value=e.apiKey),e.GeoLocation&&("Auto"==e.GeoLocation?($("#GeoAuto")[0].checked=!0,hideGeoLocElems()):e.GeoLocation.includes("lat=")?($("#GeoCoordinates")[0].checked=!0,hideGeoLocElems(),$("#GeoLocLatText").show(),$("#GeoLocLonText").show(),$("#GeoLocLatText")[0].value=e.GeoLocation.split("&")[0].split("=")[1],$("#GeoLocLonText")[0].value=e.GeoLocation.split("&")[1].split("=")[1]):e.GeoLocation.includes("id=")?($("#GeoCityID")[0].checked=!0,hideGeoLocElems(),$("#GeoCityIDText").show(),$("#GeoCityIDText")[0].value=e.GeoLocation.replace("id=","")):e.GeoLocation.includes("zip=")?($("#GeoZIPCode")[0].checked=!0,hideGeoLocElems(),$("#GeoZIPCodeText").show(),$("#GeoZIPCodeText")[0].value=e.GeoLocation.replace("zip=","")):($("#GeoCityStateCountry")[0].checked=!0,hideGeoLocElems(),$("#GeoCityStateCountryText").show(),$("#GeoCityStateCountryText")[0].value=e.GeoLocation.replace("q=",""))),e.SSID&&($("#SSID")[0].value=e.SSID),e.Password&&($("#Password")[0].value=e.Password),e.NTPServer&&($("#NTPServer")[0].value=e.NTPServer),e.Hostname&&($("#Hostname")[0].value=e.Hostname),$("#ConfigJSON")[0].innerText=JSON.stringify(e,null,4))}).done(function(){$(".modal").modal("hide")}).fail(function(){$(".modal-body")[0].innerHTML="<p>Load Failed!</p>"}),configLoading=!1}function updateConfigJSON(e){configLoading||(configJSON={},$("#ConfigJSON")[0].innerText&&(configJSON=JSON.parse($("#ConfigJSON")[0].innerText)),$('input[name="TimeZone"]:checked:enabled')[0]&&("CustomTZ"==$('input[name="TimeZone"]:checked:enabled')[0].id?($("#CustomTZOffset").show(),configJSON.TimeZone=$('input[id="CustomTZOffset"]')[0].value.replace(" ","")):($("#CustomTZOffset").hide(),configJSON.TimeZone=$('input[name="TimeZone"]:checked:enabled')[0].id)),configJSON.DST=$('input[id="DST"]')[0].checked,configJSON.Military=$('input[id="Military"]')[0].checked,$('input[name="TemperatureUnit"]:checked:enabled')[0]&&"false"==$('input[name="TemperatureUnit"]:checked:enabled')[0].dataset.value?configJSON.Metric=!1:configJSON.Metric=!0,configJSON.WeatherAnimation=$('input[id="WeatherAnimation"]')[0].checked,configJSON.Morphing=$('input[id="Morphing"]')[0].checked,$('input[name="ColorPalette"]:checked:enabled')[0]&&(configJSON.ColorPalette=$('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette),$('input[name="ColorPalette"]:checked:enabled')[0]&&$('input[name="ColorPalette"]:checked:enabled')[0].id.includes("Custom")?($("#ColorInput").show(),"radio"==e.target.type?configJSON["ColorPalette"+configJSON.ColorPalette]?($('input[id="TimeColorInput"]')[0].value=rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette].time.r,configJSON["ColorPalette"+configJSON.ColorPalette].time.g,configJSON["ColorPalette"+configJSON.ColorPalette].time.b),$('input[id="WindColorInput"]')[0].value=rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette].wind.r,configJSON["ColorPalette"+configJSON.ColorPalette].wind.g,configJSON["ColorPalette"+configJSON.ColorPalette].wind.b),$('input[id="DateColorInput"]')[0].value=rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette].date.r,configJSON["ColorPalette"+configJSON.ColorPalette].date.g,configJSON["ColorPalette"+configJSON.ColorPalette].date.b),$('input[id="WeatherColorInput"]')[0].value=rgbToHex(configJSON["ColorPalette"+configJSON.ColorPalette].weather.r,configJSON["ColorPalette"+configJSON.ColorPalette].weather.g,configJSON["ColorPalette"+configJSON.ColorPalette].weather.b)):($('input[id="TimeColorInput"]')[0].value=cc_cyan,$('input[id="WindColorInput"]')[0].value=cc_ylw,$('input[id="DateColorInput"]')[0].value=cc_grn,$('input[id="WeatherColorInput"]')[0].value=cc_wht):configJSON["ColorPalette"+$('input[name="ColorPalette"]:checked:enabled')[0].dataset.palette]={time:hexToRgb($('input[id="TimeColorInput"]')[0].value),wind:hexToRgb($('input[id="WindColorInput"]')[0].value),date:hexToRgb($('input[id="DateColorInput"]')[0].value),weather:hexToRgb($('input[id="WeatherColorInput"]')[0].value)}):$("#ColorInput").hide(),$("#BrightnessAuto")[0].checked?(configJSON.Brightness=$("#BrightnessAuto")[0].dataset.brightness,$("#Brightness").hide()):$("#BrightnessManual")[0].checked&&($("#Brightness").show(),"BrightnessManual"==e.target.id?configJSON.BrightnessValue?configJSON.BrightnessValue?$("#Brightness")[0].value=configJSON.Brightness=configJSON.BrightnessValue:$("#Brightness")[0].value=42:configJSON.BrightnessValue=$("#Brightness")[0].value=configJSON.Brightness:"Brightness"==e.target.id&&(configJSON.Brightness=$("#Brightness")[0].value,configJSON.BrightnessValue=$("#Brightness")[0].value,$('label[for="BrightnessManual"]')[0].innerText="Manual: "+$("#Brightness")[0].value)),configJSON.apiKey=$('input[id="apiKey"]')[0].value.replace(" ","").trim(),$('input[name="Location"]:checked:enabled')[0]&&("GeoAuto"==$('input[name="Location"]:checked:enabled')[0].id&&(hideGeoLocElems(),configJSON.GeoLocation="Auto"),"GeoCoordinates"==$('input[name="Location"]:checked:enabled')[0].id&&(hideGeoLocElems(),$("#BrowserLocation").show(),$("#GeoLocLatText").show(),$("#GeoLocLonText").show(),configJSON.GeoLocation=("lat="+$('input[id="GeoLocLatText"]')[0].value.trim()+"&lon="+$('input[id="GeoLocLonText"]')[0].value.trim()).replace(" ","")),"GeoCityStateCountry"==$('input[name="Location"]:checked:enabled')[0].id&&(hideGeoLocElems(),$("#GeoCityStateCountryText").show(),configJSON.GeoLocation="q="+$('input[id="GeoCityStateCountryText"]')[0].value.trim()),"GeoCityID"==$('input[name="Location"]:checked:enabled')[0].id&&(hideGeoLocElems(),$("#GeoCityIDText").show(),configJSON.GeoLocation=("id="+$('input[id="GeoCityIDText"]')[0].value.trim()).replace(" ","")),"GeoZIPCode"==$('input[name="Location"]:checked:enabled')[0].id)&&(hideGeoLocElems(),$("#GeoZIPCodeText").show(),configJSON.GeoLocation=("zip="+$('input[id="GeoZIPCodeText"]')[0].value.trim()).replace(" ","")),configJSON.SSID=$('input[id="SSID"]')[0].value.trim().replace(" ",""),configJSON.Password=$('input[id="Password"]')[0].value.trim().replace(" ",""),configJSON.NTPServer=$('input[id="NTPServer"]')[0].value.trim().replace(" ",""),configJSON.Hostname=$('input[id="Hostname"]')[0].value.trim().toLowerCase().replace(" ",""),configJSON.DateFormat="M.D.Y",$("#ConfigJSON")[0].innerText=JSON.stringify(configJSON,null,4))}function saveConfig(){$("#modalLabel")[0].innerText="Saving",$(".modal-footer").hide(),$(".modal").modal("show");var e=$("#ConfigJSON")[0].innerText,e={method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(JSON.parse(e))};fetch("/config",e).then(e=>{$(".modal").modal("hide")}).catch(e=>{$(".modal-body")[0].innerHTML="<p>Save Failed!</p>"})}function resetConfig(){$(".modal-footer").hide(),$("#modalLabel")[0].innerText="Resetting",$(".modal-body")[0].innerHTML='<div class="d-flex justify-content-center"><div class="spinner-border" role="status"><span class="visually-hidden">Loading...</span></div></div>',reset={ConfigReset:"true"};var e={method:"POST",headers:{"Content-Type":"application/json"},body:resetJSON=JSON.stringify(reset,null,4)};fetch("/reset",e).then(e=>{setTimeout(function(){},2e4),location.reload()}).catch(e=>{$(".modal-body")[0].innerHTML="<p>Reset Failed!</p>"})}function rebootClock(){$(".modal-footer").hide(),$("#modalLabel")[0].innerText="Rebooting",$(".modal-body")[0].innerHTML='<div class="d-flex justify-content-center"><div class="spinner-border" role="status"><span class="visually-hidden">Loading...</span></div></div>',reboot={rebootClock:"true"};var e={method:"POST",headers:{"Content-Type":"application/json"},body:rebootJSON=JSON.stringify(reboot,null,4)};fetch("/reboot",e).then(e=>{setTimeout(function(){},15e3),location.reload()}).catch(e=>{$(".modal-body")[0].innerHTML="<p>Reboot Failed!</p>"})}function resetModal(){$("#modalLabel")[0].innerText="Reset Config",$(".modal-body")[0].innerHTML="<p>This will clear the crrent config and restore the default values from the params.h file.</p><br><p>Are you sure you want to continue?</p>",$(".modal-footer").show(),$(".modal").modal("show"),$("#btnReset").show(),$("#btnReboot").hide()}function rebootModal(){$("#modalLabel")[0].innerText="Reboot",$(".modal-body")[0].innerHTML="<p>Are you sure you want to reboot this clock?</p>",$(".modal-footer").show(),$(".modal").modal("show"),$("#btnReset").hide(),$("#btnReboot").show()}$(window).on("load",function(){loadConfig()}),$("#Brightness").on("mousemove",function(e){$('label[for="BrightnessManual"]')[0].innerText="Manual: "+$(this).val()});
)***";
