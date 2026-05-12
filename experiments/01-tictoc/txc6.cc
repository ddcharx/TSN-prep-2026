







#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;


class Txc6 : public cSimpleModule
{
  private:

    cMessage *event = nullptr;  // pointer to the event object which we'll use for timing
    cMessage *tictocMsg = nullptr;  // variable to remember the message until we send it back
    int counter;  // Note the counter here
  public:
    virtual ~Txc6();
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc6);

Txc6::~Txc6()
{
    cancelAndDelete(event);
    cancelAndDelete(tictocMsg);
}

void Txc6::initialize()
{

    WATCH(counter);
    counter = par("limit");
    event = new cMessage("event");
    if (par("sendMsgOnInit").boolValue() == true) {
        cMessage *msg = new cMessage("tictocMsg");
        EV << "Sending initial message\n";
        send(msg, "out");
    }

}

void Txc6::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, sending message back\n";
        send(tictocMsg, "out");
        tictocMsg = nullptr;
    }
    else {
        counter--;
        if (counter <= 0) {
            EV << "Counter reached zero, ending simulation\n";
            delete msg;
            endSimulation();
            return;
        }
        EV << "Message `" << msg->getName() << "' arrived, starting to wait 1 sec...\n";
        tictocMsg = msg;
        scheduleAt(simTime() + 1.0, event);
    }
}
