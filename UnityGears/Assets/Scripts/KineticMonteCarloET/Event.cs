using UnityEngine;
using System.Collections;

namespace KineticMonteCarlo
{
    public abstract class Event
    {

        public static readonly NullEvent NULLEVENT = new NullEvent();

        protected double rate;

        private double delay;


        /*Constructor*/
        public Event(double rate)
        {
            this.rate = rate;

        }

        /*Accessor*/
        public double getRate()
        {
            return rate;
        }

        public double getDelay()
        {
            return delay;
        }

        public void setDelay(double delay)
        {
            this.delay = delay;
        }

        /*Do whatever transfer, injection, or ejection just happened*/
        public abstract void processEvent(TransitionMatrix tm);
        public abstract void renderEvent(TransitionMatrix tm);
    }


    /*THE CHILDREN*/

    public class InjectionEvent : Event
    {

        private int hemeIdx;        //for calculations/computing
        private Heme heme;          //for rendering

        /*Constuctor*/
        public InjectionEvent(double rate, int heme) : base(rate)
        {
            this.hemeIdx = heme;
        }

        /*Event Handler*/
        public override void processEvent(TransitionMatrix tm)
        {
            /*update the TM, then prep the actual Heme for rendering*/
            tm.setCurrentState(hemeIdx, true);
            heme = tm.getHemeObject(hemeIdx);
        }

        public override void renderEvent(TransitionMatrix tm)
        {
            heme.setOccupied(true);
            tm.setAnimationState(hemeIdx, true);
        }
    }

    public class EjectionEvent : Event
    {

        private int hemeIdx;            //for calculations/computing
        private Heme heme;              //for rendering

        /*Constructor*/
        public EjectionEvent(double rate, int heme) : base(rate)
        {
            this.hemeIdx = heme;
        }

        /*Event Handler*/
        public override void processEvent(TransitionMatrix tm)
        {
            tm.setCurrentState(hemeIdx, false);
            heme = tm.getHemeObject(hemeIdx);
        }

        public override void renderEvent(TransitionMatrix tm)
        {
            heme.setOccupied(false);
            tm.setAnimationState(hemeIdx, false);
        }
    }

    public class TransferEvent : Event
    {

        private int from_hemeIdx;       //for computing
        private int to_hemeIdx;         //for computing
        private Heme from_heme;         //for rendering
        private Heme to_heme;           //for rendering

        /*Constructor*/
        public TransferEvent(double rate, int from_heme, int to_heme) : base(rate)
        {
            this.from_hemeIdx = from_heme;
            this.to_hemeIdx = to_heme;
        }

        /*Event Handler*/
        public override void processEvent(TransitionMatrix tm)
        {
            tm.setCurrentState(from_hemeIdx, false);
            tm.setCurrentState(to_hemeIdx, true);

            from_heme = tm.getHemeObject(from_hemeIdx);
            to_heme = tm.getHemeObject(to_hemeIdx);

        }

        public override void renderEvent(TransitionMatrix tm)
        {
            from_heme.setOccupied(false);
            to_heme.setOccupied(true);

            tm.setAnimationState(from_hemeIdx, false);
            tm.setAnimationState(to_hemeIdx, true);
        }
    }

    public class NullEvent : Event
    {

        public NullEvent() : base(0)
        {
            setDelay(0);
        }

        public override void renderEvent(TransitionMatrix tm) { }

        public override void processEvent(TransitionMatrix tm) { }
    }
}
