# Test hashtag counting
score_card = [50, 21, 5, 0, 21]

# Scoring algorithm
prev = 0
pts = 0
move_card = [0] * len(score_card)
for c in sorted(score_card):
    if c != prev:
        pts += 1
    i = score_card.index(c)
    score_card[i] = -1
    move_card[i] = pts
    prev = c

# Print results
print score_card
print move_card
