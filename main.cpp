#include <unitTest/base/base.binding.h>
#include <unitTest/base/base.command.h>
#include <unitTest/base/base.geometry.h>
#include <unitTest/base/base.event.h>
#include <unitTest/base/base.object.property.h>

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
	VTestTask baseCommandRecall(VUnitTestM(BaseCommand, Recall), "base.command.recall");
	VTestTask baseCommandClassRecall(VUnitTestM(BaseCommand, ClassRecall), "base.command.class.recall");
	VTestTask baseGeometryPointConstruction(VUnitTestM(BaseGeometryPoint, Init), "base.geometry.point.init");
	VTestTask baseGeometryPointCalculate(VUnitTestM(BaseGeometryPoint, Calculate), "base.geometry.point.calculate");
	VTestTask baseGeometryPointCompare(VUnitTestM(BaseGeometryPoint, Compare), "base.geometry.point.compare");
	VTestTask baseGeometryRectConstruction(VUnitTestM(BaseGeometryRect, Init), "base.geometry.rect.init");
	VTestTask baseGeometryRectCompare(VUnitTestM(BaseGeometryRect, Compare), "base.geometry.rect.compare");
	VTestTask baseObjectCreate(VUnitTestM(ObjectProperty, Create), "base.object.create");
	VTestTask baseObjectRead(VUnitTestM(ObjectProperty, Read), "base.object.read");

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
	conductor.AddTask(baseCommandRecall);
	conductor.AddTask(baseCommandClassRecall);
	conductor.AddTask(baseGeometryPointConstruction);
	conductor.AddTask(baseGeometryPointCalculate);
	conductor.AddTask(baseGeometryPointCompare);
	conductor.AddTask(baseGeometryRectConstruction);
	conductor.AddTask(baseGeometryRectCompare);
	conductor.AddTask(baseObjectCreate);
	conductor.AddTask(baseObjectRead);

	conductor.StartTasks();

	return 0;
}