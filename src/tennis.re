type player =
  | PlayerOne
  | PlayerTwo;

type point =
  | Love
  | Fifteen
  | Thirty;

type pointsData = {
  playerOne: point,
  playerTwo: point,
};

type fortyData = {
  player, /* The player who have forty points */
  otherPlayerPoint: point,
};

type score =
  | Points(pointsData)
  | Forty(fortyData)
  | Deuce
  | Advantage(player)
  | Game(player);

//let startScore : score = Points({playerOne: Love, playerTwo: Love});

let scoreWhenDeuce: player => score = winner => Advantage(winner);

/* This time we infer that the function type is (player) => player */
let other = player =>
  switch (player) {
  | PlayerOne => PlayerTwo
  | PlayerTwo => PlayerOne
  };

let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) => advantagedPlayer == winner ? Game(winner) : Deuce;

/* We add a tool function to increment point */
let incrementPoint: point => option(point) =
  point =>
    switch (point) {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };

let scoreWhenForty = (current, winner) =>
  current.player == winner ?
    Game(winner) :
    (
      switch (incrementPoint(current.otherPlayerPoint)) {
      | Some(p) => Forty({...current, otherPlayerPoint: p})
      | None => Deuce
      }
    );

let pointTo = (player, point, current) =>
  switch (player) {
  | PlayerOne => {...current, playerOne: point}
  | PlayerTwo => {...current, playerTwo: point}
  };

let pointFor = (player, current) =>
  switch (player) {
  | PlayerOne => current.playerOne
  | PlayerTwo => current.playerTwo
  };

let scoreWhenPoints = (current, winner) =>
  switch (current |> pointFor(winner) |> incrementPoint) {
  | Some(np) => Points(pointTo(winner, np, current))
  | None => Forty({player: winner, otherPlayerPoint: current |> pointFor(other(winner))})
  };

let scoreWhenGame = winner => Game(winner);

let score = (current, winner) =>
  switch (current) {
  | Points(p) => scoreWhenPoints(p, winner)
  | Forty(f) => scoreWhenForty(f, winner)
  | Deuce => scoreWhenDeuce(winner)
  | Advantage(a) => scoreWhenAdvantage(a, winner)
  | Game(g) => scoreWhenGame(g)
  };

//let newGame = Points({playerOne: Love, playerTwo: Love});

let string_of_player = player =>
  switch (player) {
  | PlayerOne => "[Player One]"
  | PlayerTwo => "[Player Two]"
  };

let string_of_point = point =>
  switch (point) {
  | Love => "[Point: 0]"
  | Fifteen => "[Point: 15]"
  | Thirty => "[Point: 30]"
  };

let string_of_forty = () => "[Forty: 40]";

let string_of_score = score =>
  switch (score) {
  | Points(p) =>
    string_of_player(PlayerOne)
    ++ " : "
    ++ string_of_point(p.playerOne)
    ++ " VS "
    ++ string_of_player(PlayerTwo)
    ++ " : "
    ++ string_of_point(p.playerTwo)
  | Forty(fortyData) =>
    string_of_player(fortyData.player)
    ++ string_of_forty()
    ++ (fortyData.player == PlayerOne ? string_of_player(PlayerTwo) : string_of_player(PlayerOne))
    ++ string_of_point(fortyData.otherPlayerPoint)
  | Advantage(player) =>
    player == PlayerOne ?
      "Advantage for :" ++ string_of_player(PlayerOne) : "Advantage for :" ++ string_of_player(PlayerTwo)
  | Deuce => "Game in Deuce"
  | Game(g) => g == PlayerOne ? "Winner: " ++ string_of_player(PlayerOne) : "Winner: " ++ string_of_player(PlayerTwo)
  };
