# Web Interface Demo
This is a demo of how a Web interface using  `cubesat-framework.js`could look like. This framework was used to test send commands to the experiment simulators and to retrieve and show data from them.
## Important!
Make sure to change the IP address in `cubesat-framework.js` to your WebSocket server.

Make sure to include the  `cubesat-framework.js`  file **AFTER** the HTML elements that it should use. The framework will scan the DOM for the `cubesat` attributes and if it is included before it will not find any attributes defined later.

## Idea

The idea behind  `cubesat-framework.js` is to have a way to show telemetry and the status of the testing framework. 
### Data display
For instance to we might want to show the voltage of a package that come from unit 5 and the subsystem 11, this message might look as follows `{sat:{voltage:3.34,current:5.3,id:5,subsystem:11]}`. We could filter and display this voltage using the following HTML snippet
```html
<p class="param" data-cubesat-param="[sat.id===5;subsystem===11;sat.voltage]->sat.voltage">...</p>
```
Note that the text inside  `[...]` is executed as JavaScript and the result is check that the object exsists as can be seen with `[...sat.voltage]` where the `===` is omited, this is fine since there is still and object. Note that objects that are `false` will have not trigger without excplicitly evaluating for `===false`. 
### Commanding
Buttons simply send the data they are given.
```html
<button data-cubesat-btn='{es:{cmd:"C",id:2}}' type="button" class="btn btn-success">Request Current</button>
```
Inputs will replace the `INPUT` string with the value of the input when the user presses enter in the input field.

```html
<input data-cubesat-input="{a:{b:{c:INPUT,x:92}}}" type="number" class="form-control">
```

### Future work
There are many interesting libraries such as plotting libraries that could be integrated into this framework. This framework should not be taken as a ready product but rather a demonstration and proof of concept.
