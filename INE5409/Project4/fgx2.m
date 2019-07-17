function y = fgx2(a, xp)
    y = a(1).*sin(a(2) .+ a(3).*xp) .+ a(4).*cos(a(5) .+ a(6).*xp) .+ a(7);
end