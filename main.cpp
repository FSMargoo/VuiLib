#include <base/test/vTest.h>
#include <unitTest/base/base.event.h>

int main() {
	VTestTask baseEventTest(VUnitTestM(BaseEvent, Recall), "base.event.free_function.recall");
	VTestTask baseEventBlockTest(VUnitTestM(BaseEvent, Block), "base.event.free_function.block");
	VTestTask baseEventRemoveTest(VUnitTestM(BaseEvent, Remove), "base.event.free_function.remove");
	VTestTask baseClassEventTest(VUnitTestM(BaseEventClass, Recall), "base.event.class.recall");
	VTestTask baseClassEventBlockTest(VUnitTestM(BaseEventClass, Block), "base.event.class.block");
	VTestTask baseClassEventRemoveTest(VUnitTestM(BaseEventClass, Remove), "base.event.class.remove");
	VTestConductor conductor;
	conductor.AddTask(baseEventTest);
	conductor.AddTask(baseEventBlockTest);
	conductor.AddTask(baseEventRemoveTest);
	conductor.AddTask(baseClassEventTest);
	conductor.AddTask(baseClassEventBlockTest);
	conductor.AddTask(baseClassEventRemoveTest);
	conductor.StartTasks();


	return 0;
}