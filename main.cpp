#include <unitTest/base/base.binding.h>
#include <unitTest/base/base.event.h>

int main() {
	VTestTask baseEventTest(VUnitTestM(BaseEvent, Recall), "base.event.free_function.recall");
	VTestTask baseEventBlockTest(VUnitTestM(BaseEvent, Block), "base.event.free_function.block");
	VTestTask baseEventRemoveTest(VUnitTestM(BaseEvent, Remove), "base.event.free_function.remove");
	VTestTask baseClassEventTest(VUnitTestM(BaseEventClass, Recall), "base.event.class.recall");
	VTestTask baseClassEventBlockTest(VUnitTestM(BaseEventClass, Block), "base.event.class.block");
	VTestTask baseClassEventRemoveTest(VUnitTestM(BaseEventClass, Remove), "base.event.class.remove");
	VTestTask baseBindingValueTest(VUnitTestM(BaseBinding, ValueTest), "base.binding.value");
	VTestTask baseBindingBindingValueTest(VUnitTestM(BaseBinding, BindingValueTest), "base.binding.reference.value");
	VTestTask baseBindingUnbindTest(VUnitTestM(BaseBinding, BindingValueTest), "base.binding.unbind");
	VTestTask baseBindingValueCompareTest(VUnitTestM(BaseBinding, ValueCompareTest), "base.binding.compare.value");

	VTestConductor conductor;

	conductor.AddTask(baseEventTest);
	conductor.AddTask(baseEventBlockTest);
	conductor.AddTask(baseEventRemoveTest);
	conductor.AddTask(baseClassEventTest);
	conductor.AddTask(baseClassEventBlockTest);
	conductor.AddTask(baseClassEventRemoveTest);
	conductor.AddTask(baseBindingValueTest);
	conductor.AddTask(baseBindingBindingValueTest);
	conductor.AddTask(baseBindingUnbindTest);
	conductor.AddTask(baseBindingValueCompareTest);

	conductor.StartTasks();

	return 0;
}