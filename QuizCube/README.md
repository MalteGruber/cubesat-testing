
# CSTS Manager
This program is a manager for executing long running tests. Tests are defined as child classes of the AbstractTest class. Test classes have to implement some predefined methods and behavior but are otherwise free to implement code of arbitrary complexity.

## Test Manager Class
The test manager class keeps a list of all running tests. Tests are regiseterd to the testManager using the registerTest method
	
	manager=TestManager()
	manager.registerTest(CommandOutcomeTest("One","kjlkj"))
	manager.registerTest(CommandOutcomeTest("Two","gfdhgfd"))

## Inbound Message Propagation
When a message is received from the system is received the message is distributed to all the tests using the TestManagers `propogateMessage()` method.

## Outbound Message
The tests can issue messages by calling the `sendMessage()` function that is passed to the testManager and all tests.

## Writing a Test

Tests use the incoming objects. To select attributes of an objects a number of test functions are available.
These are `range(filter,low,high)` which returns true if a packet that matches the filter returns has a value within the range.
`boolean(filters,val=True)` and `value(filters,value)` do the same but for exact values.

These functions should be combined with `and` or `or` to create more complex tests.

Example: Suppose we want to test that `voltage` in the following object is within a range of 12±0.1 V for a device with id of 5.
	{smu:{voltage:12.312,id:5}} <-fail
	{smu:{voltage:12.312,id:5}} <-ok
	{smu:{voltage:12.312,id:5}}	<-fail
	
To achieve this we simply need the following line.
	assert(range(["smu","voltage"],12-0.1,12+0.1) and value(["smu","id"],5)

The functions will check all the messages that were received since the last call
to 
def stage()