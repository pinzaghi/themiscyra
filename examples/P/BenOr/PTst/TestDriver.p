
/*
This machine creates the 2 participants, 1 coordinator, and 2 clients 
*/

machine TestDriverAsync0 {
    start state Init {
        entry {
            launchASync((n=3, quorum=2, fm=Timeouts));
        }
    }
}

machine TestDriverAsync1 {
    start state Init {
        entry {
            launchASync((n=8, quorum=5, fm=Timeouts));
        }
    }
}

machine Participant{
    start state Init {}
}

machine TestDriverSync0 {
    start state Init {
        entry {
            launchSync((n=3, quorum=2, fm=Timeouts));
        }
    }
}

machine TestDriverSync1 {
    start state Init {
        entry {
            launchSync((n=8, quorum=5, fm=Timeouts));
        }
    }
}

fun launchASync(config: (n: int, quorum: int, fm: FailureModel))
{
    var participants: set[Process];
    var i: int;

    assert(config.quorum >= config.n/2);

    i = 0;
    while (i < config.n) {
        participants += (new Process());
        i = i + 1;
    }

    i = 0;
    while (i < config.n) {
        send participants[i], Config, (peers=participants, quorum=config.quorum, failurem=config.fm);
        i=i+1;
    }

}

fun launchSync(config: (n: int, quorum: int, fm: FailureModel))
{
    var system : BenOrSync;
    var i : int;
    var p : machine;
    var participants: set[Participant];

    assert(config.quorum >= config.n/2);
    
    i = 0;
    while (i < config.n) {
        participants += (new Participant());
        i = i + 1;
    }    
    
    system = new BenOrSync((peers=participants, quorum=config.quorum, failurem=config.fm));
 
}