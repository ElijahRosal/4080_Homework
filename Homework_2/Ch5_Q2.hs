-- Bundle operations in a type class
class Expr e where
  eval   :: e -> Int
  pretty :: e -> String

-- Concrete type: literal numbers
data Lit = Lit Int

instance Expr Lit where
  eval (Lit n) = n
  pretty (Lit n) = show n

-- Concrete type: addition
data Add = Add Lit Lit

instance Expr Add where
  eval (Add x y) = eval x + eval y
  pretty (Add x y) = "(" ++ pretty x ++ " + " ++ pretty y ++ ")"

-- Concrete type: multiplication (new type added easily)
data Mul = Mul Lit Lit

instance Expr Mul where
  eval (Mul x y) = eval x * eval y
  pretty (Mul x y) = "(" ++ pretty x ++ " * " ++ pretty y ++ ")"

-- Example usage
main :: IO ()
main = do
  let a = Add (Lit 2) (Lit 3)
      b = Mul (Lit 4) (Lit 5)
  putStrLn $ "Add evaluates to: " ++ show (eval a) ++ ", pretty: " ++ pretty a
  putStrLn $ "Mul evaluates to: " ++ show (eval b) ++ ", pretty: " ++ pretty b
