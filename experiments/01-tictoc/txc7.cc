







#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;


class Txc7 : public cSimpleModule
{
  private:

    cMessage *event = nullptr;  // pointer to the event object which we'll use for timing
    cMessage *tictocMsg = nullptr;  // variable to remember the message until we send it back
    int counter;  // Note the counter here
  public:
    virtual ~Txc7();
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc7);

Txc7::~Txc7()
{
    cancelAndDelete(event);
    cancelAndDelete(tictocMsg);
}

void Txc7::initialize()
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

void Txc7::handleMessage(cMessage *msg)
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
        if (uniform(0, 1) < 0.1) {
                   EV << "\"Losing\" message\n";
                   delete msg;
               }
        simtime_t delay = par("delayTime");
        EV << "Message `" << msg->getName() << "' arrived, starting to wait" << delay << "secs...\n";
        tictocMsg = msg;
        scheduleAt(simTime() + 1.0, event);
    }
}
