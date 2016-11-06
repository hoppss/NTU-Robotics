function binary_vision (img_path)
  threshold = 150;
  binarized = thresholding (img_path, threshold);
  [segmented, region] = segmentation (binarized);
  [xc, yc, angle] = areaDescriptor (segmented, region)
endfunction

function binarized = thresholding (img_path, threshold)
  I = imread(img_path);
  mask = [1 1 1 ; 1 1 1; 1 1 1];
  I = imerode(I, mask);
  I = imdilate(I, mask);
  [xlen, ylen] = size(I);
  binarized = zeros(xlen, ylen);
  for x = 1:xlen
    for y = 1:ylen
      if I(x, y) > threshold
        binarized(x, y) = 255;
      end
    end
  end
endfunction

function [segmented, region] = segmentation (binarized)
  [xlen, ylen] = size(binarized);
  segmented = binarized;
  region = 0;
  for x = 1:xlen
    for y = 1:ylen
      if segmented(x, y) == 255
        region += 1;
        segmented = region_growing(segmented, region, x, y);
      end
    end
  end
endfunction

function segmented = region_growing (segmented, region, x, y)
  stack = zeros(500,2);
  ptr = 0;
  [xlen, ylen] = size(segmented);
  segmented(x, y) = region;
  # push (x,y)
  ptr += 1; stack(ptr, 1) = x; stack(ptr, 2) = y;
  
  while ptr > 0 
    if y < ylen && segmented(x, y+1) == 255
      segmented(x, y+1) = region;
      ptr += 1; stack(ptr, 1) = x; stack(ptr, 2) = y + 1;
    end  
    if x > 1 && segmented(x-1, y) == 255
      segmented(x-1, y) = region;
      ptr += 1; stack(ptr, 1) = x - 1; stack(ptr, 2) = y;
    end  
    
    if y > 1 && segmented(x, y-1) == 255
      segmented(x, y-1) = region;
      ptr += 1; stack(ptr, 1) = x; stack(ptr, 2) = y - 1;
    end
    if x < xlen && segmented(x+1, y) == 255
      segmented(x+1, y) = region;
      ptr += 1; stack(ptr, 1) = x + 1; stack(ptr, 2) = y;
    end
    # pop
    x = stack(ptr, 1); y = stack(ptr, 2); ptr -= 1;
  end
endfunction

function [XC, YC, ANGLE] = areaDescriptor (segmented, region)
  XC = YC = ANGLE = [];
  for i = 1:region
    [row col] = find(segmented == i); # vectors of x & y indices of region i 
    yc = sum(row) / size(row, 1);
    xc = sum(col) / size(col, 1);
    angle = 0.5 * atan2(2 * cMoment(col, row, xc, yc, 1, 1), cMoment(col, row, xc, yc, 2, 0) - cMoment(col, row, xc, yc, 0, 2));
    XC = [XC; xc];
    YC = [YC; yc];
    ANGLE = [ANGLE; angle];
  end
endfunction

function u = cMoment(x, y, xc, yc, k, j)
    xu = x - xc;
    yu = y - yc;
    u = sum ( (xu .^ k) .* (yu .^ j));
end