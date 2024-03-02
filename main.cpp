#include <base/LINQ/vlinq.h>
#include <base/event/vevent.h>

void Test(int a, VBase::VObject* Object) {
	printf("%d", a);
}

int main() {
	VBase::VEvent<int> Event;
	Event.Subscribe(Test);
	Event.Unsubscribe(Test);
	Event.Send(4, nullptr);

	return 0;
}