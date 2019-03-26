open Redux_Types;


module Make = (ModelImpl: Model) => {
    type state = ModelImpl.state;
    type action = ModelImpl.action;

    let updater = ModelImpl.updater;

    let currentState: ref(state) = ref(ModelImpl.initialState);

    let getState = () => currentState^;

    let _subscribers = ref([]);

    let poll = () => ();

    let subscribe = (fn) => {

        _subscribers := [fn, ..._subscribers^];

        () => {
            _subscribers := List.filter(f => f != fn, _subscribers^);
        }
    };

    let dispatch = (action) => {
        let (newState, effect) = updater(currentState^, action);
        currentState := newState;

        Effect.run(effect);

        List.iter((f) => f(newState), _subscribers^);
    };
};
