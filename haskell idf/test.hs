import Data.Char
import Data.List
import Data.String
import Data.Tuple
import Data.Eq
import Data.Bool
import Data.Ord
import System.IO
import Text.Show

--------------------------------------------------------------------------------
--                                 IO                                         --
--------------------------------------------------------------------------------

loadToken :: String -> IO()
loadToken s = do  
        contents <- readFile s
        putStr (show (tokenizer contents) ++ "\n")   

loadParse :: String -> IO()
loadParse s = do  
        contents <- readFile s
        putStr (show (parser contents) ++ "\n")            
        
--------------------------------------------------------------------------------
--                                 DATA                                       --
--------------------------------------------------------------------------------
type StringNumber = String

data IDF = IDFNode String [IDF] [(String, String)] deriving Show
        
--------------------------------------------------------------------------------
--                                 TOKENIZER                                  --
--------------------------------------------------------------------------------

data TokState = TokInit |
                TokNewline |
                TokWhitespace |
                TokWord |
                TokNumber |
                TokNumberPoint |
                TokList |
                TokString
                
tokenizer :: String -> [String]
tokenizer s = snd (tokenizer_h TokInit s)

-- De daadwerkelijke tokenizer               
tokenizer_h :: TokState -> String -> (String, [String])
tokenizer_h TokInit []                          = ([], [])
tokenizer_h TokInit (x:xs)  | elem x "[,\n"     = ([], [[x]] ++ snd(tokenizer_h TokInit xs))
                            | elem x "\t "      = ([], [x:result2] ++ snd(tokenizer_h TokInit rest2))
                            | x == '.'          = ([], [x:result3] ++ snd(tokenizer_h TokInit rest3))
                            | x == '\"'         = ([], [x:result4] ++ snd(tokenizer_h TokInit rest4))
                            | isDigit(x)        = ([], [x:result5] ++ snd(tokenizer_h TokInit rest5))
                            | isLetter(x)       = ([], [x:result6] ++ snd(tokenizer_h TokInit rest6))
                            where   (rest2, result2:[]) = tokenizer_h TokWhitespace    xs
                                    (rest3, result3:[]) = tokenizer_h TokNumberPoint   xs
                                    (rest4, result4:[]) = tokenizer_h TokString        xs
                                    (rest5, result5:[]) = tokenizer_h TokNumber        xs
                                    (rest6, result6:[]) = tokenizer_h TokWord          xs
                                    

tokenizer_h TokWhitespace (x:xs)    | elem x "\t "  = (rest1, [x:result1]) 
                                    | otherwise     = ((x:xs), [""])
                                where (rest1, result1:[]) = tokenizer_h TokWhitespace xs
                                
tokenizer_h TokString (x:xs)    | x == '\"' = (xs, [[x]]) 
                                | otherwise = (rest1, [x:result1]) 
                            where (rest1, result1:[]) = tokenizer_h TokString xs
                            
tokenizer_h TokNumber (x:xs)    | isDigit x = (rest1, [x:result1])
                                | x == '.'  = (rest2, [x:result2])
                                | otherwise = ((x:xs), [""])
                            where   (rest1, result1:[]) = tokenizer_h TokNumber xs
                                    (rest2, result2:[]) = tokenizer_h TokNumberPoint xs
                                    
tokenizer_h TokNumberPoint (x:xs)   | isDigit x = (rest1, [x:result1]) 
                                    | otherwise = ((x:xs), [""])
                                where (rest1, result1:[]) = tokenizer_h TokNumberPoint xs
                                
tokenizer_h TokWord (x:xs)  | isLetter x || isDigit x || elem x "_-"      = (rest1, [x:result1]) 
                            | otherwise                                 = ((x:xs), [""])
                            where (rest1, result1:[]) = tokenizer_h TokWord xs
                            
tokenizer_h a [] = ([], [""])

--------------------------------------------------------------------------------
--                                 PARSER                                     --
--------------------------------------------------------------------------------
                                
parser :: String -> IDF
parser s = IDFNode "" xs ys
            where (stf, xs, ys) =  parser_h "" (tokenizer s)

--              A   -> Word ( \t) \n
--                  -> Word ( \t) Something \n
--                  -> ( \t) Word ( \t) \n
--
--
--
--

isCategory (x:[])       = True
isCategory (x:y:[])     = True
isCategory (x:y:z:xs)   = (y == "\n" || elem (head y) " \t" && z == "\n")

isVariable (x:[])       = False
isVariable (x:y:[])     = False
isVariable (x:y:z:xs)   = (elem (head y) " \t" && z /= "\n") 
                
parser_h :: String -> [String] -> ([String], [IDF], [(String, String)])
parser_h level []                                               = ([], [], [])
parser_h level (x:xs)       | x == "\n"                         = parser_h level xs       
                            | null level && isCategory (x:xs)   = (rest3, catresult1 ++ iresult3, vresult3)
                            | null level && isVariable (x:xs)   = (rest1, iresult1, varresult1 ++ vresult1)
                            | level == x && isCategory xs       = (rest4, catresult2 ++ iresult4, vresult4)       
                            | level == x && isVariable xs       = (rest2, iresult2, varresult2 ++ vresult2)
                            | otherwise                         = ((x:xs), [], [])
                        where   (varrest1, varnonresult1, varresult1)   = parser_h_var (x:xs)
                                (varrest2, varnonresult2, varresult2)   = parser_h_var xs
                                (catrest1, catresult1, catnonresult1)   = parser_h_cat x level xs
                                (catrest2, catresult2, catnonresult2)   = parser_h_cat y level ys
                                (rest1, iresult1, vresult1)             = parser_h level varrest1
                                (rest2, iresult2, vresult2)             = parser_h level varrest2
                                (rest3, iresult3, vresult3)             = parser_h level catrest1
                                (rest4, iresult4, vresult4)             = parser_h level catrest2
                                (y:ys)                                  = xs

parser_h_var :: [String] -> ([String], [IDF], [(String, String)])
parser_h_var (x:y:z:xs) = (xs, [], [(x, z)])

parser_h_cat :: String -> String -> [String] -> ([String], [IDF], [(String, String)])
parser_h_cat name level (x:xs)  | x == "\n" &&  notElem (head y) " \n"  = (xs,      [IDFNode name [] []],   [])
                                | x == "\n" && length y > length level  = (rest,    [node1],                [])                                
                                | x == "\n"                             = (xs,      [IDFNode name [] []],   [])
                                | otherwise                             = parser_h_cat name level xs
                            where   (y:ys)                  = xs
                                    (rest, idfs, vars)      = parser_h y xs 
                                    node1                   = (IDFNode name idfs vars) 
                                    
                                    
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
                        
