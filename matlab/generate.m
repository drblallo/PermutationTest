function generate()
    t = 0;
    for a = 1:1000
        x = gen1();
        y = gen2();
        t = t + (hotell2(x, y) < 0.05);
    end
    t
end

function val = gen1()
    val = normrnd(1, 1, 500, 3);
end

function val = gen2()
    val = zeros(500, 3);
    for a = 1:500
        val(a, 1) = normrnd(1.05, 2);
        val(a, 3) = normrnd(0.95, 3);
        val(a, 2) = normrnd(1, 1);
    end
    
end


