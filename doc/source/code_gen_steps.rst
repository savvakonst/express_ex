LLVM IR code generation steps
====================================


methods like visitEnter() was omitted

.. uml::

    @startuml
    User-> "Express_ex:": parseText
    activate "Express_ex:"

    "Express_ex:" -> "KexParser:"**: new
    activate "KexParser:"
    "KexParser:"  -> "BodyTemplate:" ** : new
    "KexParser:" -> "ExValue_ifs: first set" ** :
    deactivate "KexParser:"


    "Express_ex:" -> "KexParser:":getCurrentBody
    activate "KexParser:"
    "KexParser:"->"Express_ex:":BodyTemplate:
    deactivate "KexParser:"






    "Express_ex:" -> User: bool:
    deactivate "Express_ex:"

    /'----------------------------------------'/

    User-> "Express_ex:": getParameterLinkNamesMap
    activate "Express_ex:"

    "Express_ex:" -> "BodyTemplate:":: getParameterLinkNames
    activate "BodyTemplate:"
    "BodyTemplate:"->"Express_ex:"
    deactivate "BodyTemplate:"

    "Express_ex:" -> User:
    deactivate "Express_ex:"

    /'----------------------------------------'/

    User-> "Express_ex:": setParameters
    activate "Express_ex:"
    "Express_ex:"-> "BodyTemplate:" : genBodyByTemplate
    activate "BodyTemplate:"
    "BodyTemplate:"-> "Body:\nbody_" ** : new

    "BodyTemplate:" -> "ExValue_ifs: first set": genBodyVisitExit
    activate "ExValue_ifs: first set"
    "ExValue_ifs: first set" -> "ExValue_ifs:second set" **
    deactivate "ExValue_ifs: first set"



    "BodyTemplate:"->"Express_ex:" : Body:

    deactivate "BodyTemplate:"

    "Express_ex:" -> "Body:\nbody_": simplify
    activate "Body:\nbody_"
    "Body:\nbody_" -> "ExValue_ifs:second set":markUnusedVisitEnter
    /'markUnusedVisitEnter
    deactivate "Body:\nbody_"


    "Express_ex:" -> User: bool:


    deactivate "Express_ex:"

    @enduml



Operation_ifs class description:
##########################

.. doxygenclass:: Operation_ifs
    :members:
    :protected-members:
    :private-members:
    :allow-dot-graphs:


corresponding diagrams:
##########################

.. uml::

    @startuml
    activate Body

        Body -> Operation_ifs: markUnusedVisitEnter:

        activate Operation_ifs

            Operation_ifs ->Operation_ifs:   commonMarkUnusedVisitEnter(visitor_stack);
            Operation_ifs ->Operation_ifs:   visitEnterSetupBuffer(visitor_stack);
            Operation_ifs ->Operation_ifs:   visitEnterStackUpdate(visitor_stack);

        deactivate Operation_ifs

    deactivate Body
    @enduml